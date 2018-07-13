'**********************************************************************************************
'* AdvancedHMI Driver
'* http://www.advancedhmi.com
'* Ethernet/IP for ControlLogix
'*
'* Archie Jacobs
'* Manufacturing Automation, LLC
'* support@advancedhmi.com
'* 14-DEC-10
'*
'*
'* Copyright 2010,2013 Archie Jacobs
'*
'* This class implements the Ethernet/IP protocol.
'*
'* NOTICE : If you received this code without a complete AdvancedHMI solution
'* please report to sales@advancedhmi.com
'*
'* Distributed under the GNU General Public License (www.gnu.org)
'*
'* This program is free software; you can redistribute it and/or
'* as published by the Free Software Foundation; either version 2
'* of the License, or (at your option) any later version.
'*
'* This program is distributed in the hope that it will be useful,
'* but WITHOUT ANY WARRANTY; without even the implied warranty of
'* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
'* GNU General Public License for more details.

'* You should have received a copy of the GNU General Public License
'* along with this program; if not, write to the Free Software
'* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
'*
'* 23-MAY-12  Renamed PolledAddress* variables to Subscription* for clarity
'* 23-MAY-12  Created ComError event and check if subscription exists before sending error
'* 24-SEP-12  Added array reading optimization for subscriptions
'* 11-OCT-12  Do not optimize complex types, such as strings
'* 07-NOV-12  Array tags not sorting by element and caused number of elements to read to be wrong
'* 22-JAN-13  Added BasePLCAddress to Subscription info and used to sort arrays properly
'*******************************************************************************************************
Imports System.ComponentModel.Design

'<Assembly: system.Security.Permissions.SecurityPermissionAttribute(system.Security.Permissions.SecurityAction.RequestMinimum)> 
'<Assembly: CLSCompliant(True)> 
<System.ComponentModel.DefaultEvent("DataReceived")> _
Public Class EthernetIPforCLXCom
    Inherits System.ComponentModel.Component
    Implements AdvancedHMIDrivers.IComComponent

    '* Create a common instance to share so multiple driver instances can be used in a project
    Protected Shared DLL(100) As MfgControl.AdvancedHMI.Drivers.CIPforCLX
    Protected MyDLLInstance As Integer
    Private Shared ObjectIDs As Int64
    Private MyObjectID As Int64
    Private EventHandlerDLLInstance As Integer

    'Private SubscriptionsStopWaitHandle As System.Threading.EventWaitHandle


    Public Event DataReceived As EventHandler(Of MfgControl.AdvancedHMI.Drivers.Common.PlcComEventArgs)
    Public Event ComError As EventHandler(Of MfgControl.AdvancedHMI.Drivers.Common.PlcComEventArgs)
    Public Event ConnectionEstablished As EventHandler
    Public Event ConnectionClosed As EventHandler
    Public Event UnsolictedMessageRcvd As EventHandler

    Private SubscriptionLock As New Object

    '* keep the original address by ref of low TNS byte so it can be returned to a linked polling address
    '* 2-AUG-13 Removed the Shared
    Private Requests(255) As List(Of CLXAddressRead)
    'Private Responses(255) As MfgControl.AdvancedHMI.Drivers.Common.PlcComEventArgs

    Private waitHandle(255) As System.Threading.EventWaitHandle


#Region "Constructor"
    Public Sub New(ByVal container As System.ComponentModel.IContainer)
        MyClass.New()

        'Required for Windows.Forms Class Composition Designer support
        container.Add(Me)
    End Sub

    Public Sub New()
        MyBase.New()

        ObjectIDs += 1
        MyObjectID = ObjectIDs

        For index = 0 To 255
            waitHandle(index) = New System.Threading.EventWaitHandle(False, System.Threading.EventResetMode.AutoReset)
        Next

        '* set to maximum size
        m_CIPConnectionSize = 508
    End Sub

    '***************************************************************
    '* Create the Data Link Layer Instances
    '* if the IP Address is the same, then resuse a common instance
    '***************************************************************
    Protected Overridable Sub CreateDLLInstance()
        If Me.DesignMode Or m_IPAddress = "0.0.0.0" Then Exit Sub

        If DLL(0) IsNot Nothing Then
            '* At least one DLL instance already exists,
            '* so check to see if it has the same IP address
            '* if so, reuse the instance, otherwise create a new one
            Dim i As Integer
            While DLL(i) IsNot Nothing AndAlso (DLL(i).EIPEncap.IPAddress <> m_IPAddress OrElse DLL(i).ProcessorSlot <> m_ProcessorSlot) AndAlso i < 100
                i += 1
            End While
            MyDLLInstance = i
        End If

        If MyDLLInstance > DLL.Length Then
            '* TODO:
            System.Windows.Forms.MessageBox.Show("A limit of " & DLL.Length & " driver instances")
            Exit Sub
        End If

        If DLL(MyDLLInstance) Is Nothing Then
            Try
                DLL(MyDLLInstance) = New MfgControl.AdvancedHMI.Drivers.CIPforCLX
            Catch ex As Exception
                System.Windows.Forms.MessageBox.Show(ex.Message)
            End Try

            DLL(MyDLLInstance).EIPEncap.IPAddress = m_IPAddress
            DLL(MyDLLInstance).EIPEncap.Port = m_Port
            '* Set to 1 for backplane path to processor slot
            DLL(MyDLLInstance).ConnectionPathPort = 1
            DLL(MyDLLInstance).ProcessorSlot = m_ProcessorSlot
            DLL(MyDLLInstance).ConnectionByteSize = m_CIPConnectionSize
        End If

        '* Have we already attached event handler to this data link layer?
        If EventHandlerDLLInstance <> (MyDLLInstance + 1) Then
            '* If event handler to another layer has been created, remove them
            If EventHandlerDLLInstance > 0 Then
                RemoveHandler DLL(EventHandlerDLLInstance).DataReceived, AddressOf DataLinkLayerDataReceived
                RemoveHandler DLL(EventHandlerDLLInstance).ComError, AddressOf DataLinkLayerComError
                RemoveHandler DLL(EventHandlerDLLInstance).ConnectionEstablished, AddressOf CIPConnectionEstablished
                RemoveHandler DLL(EventHandlerDLLInstance).ConnectionClosed, AddressOf CIPConnectionClosed
            End If

            AddHandler DLL(MyDLLInstance).DataReceived, AddressOf DataLinkLayerDataReceived
            AddHandler DLL(MyDLLInstance).ComError, AddressOf DataLinkLayerComError
            AddHandler DLL(MyDLLInstance).ConnectionEstablished, AddressOf CIPConnectionEstablished
            AddHandler DLL(MyDLLInstance).ConnectionClosed, AddressOf CIPConnectionClosed
            EventHandlerDLLInstance = MyDLLInstance + 1

            '* Track how many instanced use this DLL, so we know when to dispose
            DLL(MyDLLInstance).ConnectionCount += 1
        End If
    End Sub

    Private Sub RemoveDLLConnection(ByVal instance As Integer)
        '* The handle linked to the DataLink Layer has to be removed, otherwise it causes a problem when a form is closed
        If DLL(instance) IsNot Nothing Then
            RemoveHandler DLL(instance).DataReceived, AddressOf DataLinkLayerDataReceived
            RemoveHandler DLL(instance).ComError, AddressOf DataLinkLayerComError
            RemoveHandler DLL(instance).ConnectionEstablished, AddressOf CIPConnectionEstablished
            RemoveHandler DLL(instance).ConnectionClosed, AddressOf CIPConnectionClosed
            EventHandlerDLLInstance = 0

            DLL(instance).ConnectionCount -= 1

            If DLL(instance).ConnectionCount <= 0 Then
                DLL(instance).Dispose()
                DLL(instance) = Nothing
            End If
        End If
    End Sub


    'Component overrides dispose to clean up the component list.
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        '* Stop the subscription thread
        StopSubscriptions = True

        '* Wait for the thread to exit
        If SubscriptionThread IsNot Nothing AndAlso SubscriptionThread.IsAlive Then
            SubscriptionThread.Join(5000)
        End If

        RemoveDLLConnection(MyDLLInstance)

        MyBase.Dispose(disposing)
    End Sub
#End Region

#Region "Properties"
    Private m_ProcessorSlot As Integer
    Public Property ProcessorSlot() As Integer
        Get
            Return m_ProcessorSlot
        End Get
        Set(ByVal value As Integer)
            m_ProcessorSlot = value
            If DLL(MyDLLInstance) IsNot Nothing Then
                DLL(MyDLLInstance).ProcessorSlot = value
            End If
        End Set
    End Property

    Private m_IPAddress As String = "192.168.0.10"
    Private m_IPIniFile As String = ""
    Public Property IPAddress() As String
        Get
            If Not String.IsNullOrEmpty(m_IPIniFile) Then
                Return m_IPIniFile
            Else
                Return m_IPAddress
            End If
        End Get

        Set(ByVal value As String)
            If Not String.IsNullOrEmpty(value) Then
                If m_IPAddress <> value Then
                    If value.IndexOf(".ini", 0, StringComparison.CurrentCultureIgnoreCase) > 0 Then
                        Try
                            If Not Me.DesignMode Then
                                Dim p As New IniParser(value)
                                m_IPAddress = p.GetSetting("IPADDRESS")
                            End If
                            m_IPIniFile = value
                        Catch ex As Exception
                            System.Windows.Forms.MessageBox.Show(ex.Message)
                            'Dim dbg = 0
                            Exit Property
                        End Try
                    Else
                        m_IPAddress = value
                        m_IPIniFile = ""
                    End If
                End If
            Else
                m_IPAddress = ""
                m_IPIniFile = ""
            End If


            If EventHandlerDLLInstance = (MyDLLInstance + 1) Then
                RemoveDLLConnection(MyDLLInstance)
            End If


            '* If a new instance needs to be created, such as a different IP Address
            CreateDLLInstance()


            If DLL(MyDLLInstance) IsNot Nothing Then
                DLL(MyDLLInstance).EIPEncap.IPAddress = m_IPAddress
            End If
            ' End If
        End Set
    End Property

    Private m_Port As Integer = &HAF12
    Public Property Port As Integer
        Get
            Return m_Port
        End Get
        Set(value As Integer)
            If value <> m_Port Then
                '* If this been attached to a DLL, then remove first
                If EventHandlerDLLInstance = (MyDLLInstance + 1) Then
                    RemoveDLLConnection(MyDLLInstance)
                End If

                '* Limit the value to 0-65535
                m_Port = Math.Max(0, Math.Min(value, 65535))

                '* If a new instance needs to be created, such as a different IP Address
                CreateDLLInstance()


                If DLL(MyDLLInstance) Is Nothing Then
                Else
                    DLL(MyDLLInstance).EIPEncap.Port = value
                End If
            End If
        End Set
    End Property

    '* This is the CIP connection size used in the Forward Open
    Private m_CIPConnectionSize As Integer
    Public Property CIPConnectionSize As Integer
        Get
            Return m_CIPConnectionSize
        End Get
        Set(value As Integer)
            m_CIPConnectionSize = Math.Min(value, 511)
            m_CIPConnectionSize = Math.Max(100, m_CIPConnectionSize)
        End Set
    End Property


    Private m_PollRateOverride As Integer = 500
    <System.ComponentModel.Category("Communication Settings")> _
    Public Property PollRateOverride() As Integer
        Get
            Return m_PollRateOverride
        End Get
        Set(ByVal value As Integer)
            If value >= 0 Then
                m_PollRateOverride = value
            End If
        End Set
    End Property

    '**************************************************************
    '* Stop the polling of subscribed data
    '**************************************************************
    Private m_DisableSubscriptions As Boolean
    Public Property DisableSubscriptions() As Boolean Implements IComComponent.DisableSubscriptions
        Get
            Return m_DisableSubscriptions
        End Get
        Set(ByVal value As Boolean)
            m_DisableSubscriptions = value
        End Set
    End Property


    '**********************************************************************************************************
    '* Do not use the MultipleServiceRequest for controller that do not support it such as the Micro800Series
    '**********************************************************************************************************
    Private m_DisableMultiServiceRequest As Boolean
    Public Property DisableMultiServiceRequest As Boolean
        Get
            Return m_DisableMultiServiceRequest
        End Get
        Set(value As Boolean)
            m_DisableMultiServiceRequest = value
        End Set
    End Property

    '**************************************************
    '* Its purpose is to fetch
    '* the main form in order to synchronize the
    '* notification thread/event
    '**************************************************
    Private m_SynchronizingObject As System.ComponentModel.ISynchronizeInvoke
    '* do not let this property show up in the property window
    ' <System.ComponentModel.Browsable(False)> _
    '* Copied from the Timer.cs file of the .NET source code
    Public Property SynchronizingObject() As System.ComponentModel.ISynchronizeInvoke
        Get
            If m_SynchronizingObject Is Nothing AndAlso DesignMode Then
                Dim host As IDesignerHost = DirectCast(GetService(GetType(IDesignerHost)), IDesignerHost)
                If host IsNot Nothing Then
                    Dim baseComponent As Object = host.RootComponent
                    If baseComponent IsNot Nothing AndAlso TypeOf baseComponent Is System.ComponentModel.ISynchronizeInvoke Then
                        Me.SynchronizingObject = DirectCast(baseComponent, System.ComponentModel.ISynchronizeInvoke)
                    End If
                End If
            End If

            Return m_SynchronizingObject
        End Get

        Set(value As System.ComponentModel.ISynchronizeInvoke)
            m_SynchronizingObject = value
        End Set
    End Property
#End Region

#Region "Public Methods"
    Public Sub CloseConnection()
        '* v3.97e - changed to RemoveDLLConnection
        '* 23-JAN-15
        'If DLL(MyDLLInstance) IsNot Nothing Then
        '    DLL(MyDLLInstance).ForwardClose()
        'End If
        RemoveDLLConnection(MyDLLInstance)
    End Sub

    '**********************************************************************************
    '* Synchronous read operation that will wait for result before returning to caller
    '**********************************************************************************
    Public Function Read(ByVal startAddress As String, ByVal numberOfElements As Integer) As String() Implements IComComponent.Read
        Dim SequenceNumber As Integer = BeginRead(startAddress, numberOfElements)
        Dim TransactionByte As Integer = SequenceNumber And 255

        Dim Signalled As Boolean = waitHandle(TransactionByte).WaitOne(2000)

        If Signalled Then
            If Requests(TransactionByte)(0).Response IsNot Nothing Then
                If Requests(TransactionByte)(0).Response.Values IsNot Nothing Then
                    Dim d(Requests(TransactionByte)(0).Response.Values.Count - 1) As String
                    If Requests(TransactionByte)(0).Response.ErrorId = 0 Then
                        '* a Bit Array will return number of elements rounded up to 32, so return only the amount requested
                        If Requests(TransactionByte)(0).Response.Values.Count > numberOfElements Then
                            Dim v(numberOfElements - 1) As String
                            For i As Integer = 0 To v.Length - 1
                                v(i) = Requests(TransactionByte)(0).Response.Values(i)
                            Next i
                            Return v
                        Else
                            '* Is the start address a bit number?
                            If Requests(TransactionByte)(0).BitNumber >= 0 Then
                                Dim ElementNumber, BitNumber As Integer
                                Dim BitsPerElement As Integer = 32
                                Select Case Requests(TransactionByte)(0).AbreviatedDataType
                                    Case &HC3 '* INT Value read 
                                        BitsPerElement = 16
                                    Case &HC4 '* DINT Value read
                                        BitsPerElement = 32
                                    Case &HC2 '* SINT
                                        BitsPerElement = 8
                                End Select

                                '* Convert values to bits
                                For i = 0 To d.Length - 1
                                    ElementNumber = Convert.ToInt32(Math.Floor((Requests(TransactionByte)(0).BitNumber + i) / BitsPerElement))
                                    BitNumber = (Requests(TransactionByte)(0).BitNumber + i) - ElementNumber * BitsPerElement
                                    If Requests(TransactionByte)(0).Response.Values.Count > ElementNumber Then
                                        'd(i) = Convert.ToString((Convert.ToInt32(Requests(SequenceNumber).Response.Values(ElementNumber)) And Convert.ToInt32(2 ^ BitNumber)))
                                        '* V3.97d - was not returning True/False
                                        d(i) = Convert.ToString(((Convert.ToInt32(Requests(TransactionByte)(0).Response.Values(ElementNumber)) And Convert.ToUInt32(2 ^ BitNumber))) > 0)
                                    End If
                                Next
                            Else
                                Requests(TransactionByte)(0).Response.Values.CopyTo(d, 0)
                            End If

                            Return d
                        End If
                    Else
                        Throw New MfgControl.AdvancedHMI.Drivers.Common.PLCDriverException("Read Failed. " & Requests(TransactionByte)(0).Response.ErrorMessage & ",  Status Code=" & Requests(TransactionByte)(0).Response.ErrorId)
                    End If
                Else
                    Throw New MfgControl.AdvancedHMI.Drivers.Common.PLCDriverException("Read Failed - No Values extracted")
                End If
            Else
                Throw New MfgControl.AdvancedHMI.Drivers.Common.PLCDriverException("Read Failed - No Reponse Data")
            End If
        Else
            'Requests(SequenceNumber).Response.ErrorId = result
            Throw New MfgControl.AdvancedHMI.Drivers.Common.PLCDriverException("No response from PLC.") ' & DecodeMessage(result))
        End If
    End Function

    Public Function Read(ByVal startAddress As String) As String
        Return Read(startAddress, 1)(0)
    End Function


    '*********************************************************************
    '* Asynchronous Read that returns Transaction Number(Sequence Number)
    '*********************************************************************
    Private ReadLock As New Object
    Public Function BeginRead(ByVal startAddress As String, ByVal numberOfElements As Integer) As Integer Implements IComComponent.BeginRead
        SyncLock (ReadLock)
            Try
                Return BeginRead(New CLXAddressRead(startAddress), numberOfElements)
            Catch ex As Exception
                Throw ex
            End Try
        End SyncLock
    End Function

    '**************************************************************************************
    '* This function is used for asynchronous reads
    '**************************************************************************************
    Public Function BeginRead(ByVal address As CLXAddressRead, ByVal numberOfElements As Integer) As Integer
        Try
            address.TransactionNumber = CUShort(DLL(MyDLLInstance).GetNextTransactionNumber(32767))

            Dim TransactionByte As Integer = address.TransactionNumber And 255
            'If waitHandle(TransactionByte) Is Nothing Then
            '    waitHandle(TransactionByte) = New System.Threading.EventWaitHandle(False, System.Threading.EventResetMode.AutoReset)
            'Else
            waitHandle(TransactionByte).Reset()
            'End If
        Catch ex As Exception
            Throw ex
        End Try

        Requests(address.TransactionNumber And 255) = New List(Of CLXAddressRead)
        Requests(address.TransactionNumber And 255).Add(address)
        Requests(address.TransactionNumber And 255)(0).Response = Nothing

        Return DLL(MyDLLInstance).ReadTagValue(address, numberOfElements, CUShort(address.TransactionNumber), MyObjectID)
    End Function

    '**************************************************************************************
    '* This function is used for asynchronous reads of multiple tags
    '**************************************************************************************
    Public Function BeginReadMultiple(ByVal addresses As List(Of CLXAddressRead)) As Integer
        Dim TransactionByte As Integer
        Dim SequenceNumber As UShort
        'Try
        SequenceNumber = CUShort(DLL(MyDLLInstance).GetNextTransactionNumber(32767))
        TransactionByte = SequenceNumber And 255
        'Catch ex As Exception
        'Throw ex
        'End Try

        '* Save this information for the response
        Requests(TransactionByte) = New List(Of CLXAddressRead)
        For index = 0 To addresses.Count - 1
            Requests(TransactionByte).Add(addresses(index))
            Requests(TransactionByte)(index).Response = Nothing
        Next

        waitHandle(TransactionByte).Reset()

        'Dim a As New List(Of MfgControl.AdvancedHMI.Drivers.CLXAddress)
        'For Each ad In addresses
        '    Dim b As New MfgControl.AdvancedHMI.Drivers.CLXAddress
        '    b.TagName = ad.TagName
        '    a.Add(b)
        'Next

        ' DLL(MyDLLInstance).ReadMultipleTagValues(a, TransactionNumber, MyObjectID)


        DLL(MyDLLInstance).ReadMultipleTagValues(addresses, SequenceNumber, MyObjectID)

        Return SequenceNumber
    End Function


    '*************************************************
    '* Retreive the list of tags in the ControlLogix
    '* This is operation waits until all tags are
    '*  retreived before returning
    '*************************************************
    Public Function GetTagList() As MfgControl.AdvancedHMI.Drivers.CLXTag()
        '* We must get the sequence number from the DLL
        '* and save the read information because it can comeback before this
        '* information gets put in the PLCAddressByTNS array
        Dim SequenceNumber As Integer = DLL(MyDLLInstance).GetNextTransactionNumber(32767) ' TNS1.GetNextNumber("c", MyObjectID)

        Dim SequenceByte As Integer = SequenceNumber And 255
        Requests(SequenceByte) = New List(Of CLXAddressRead)
        Requests(SequenceByte).Add(New AdvancedHMIDrivers.CLXAddressRead)
        Requests(SequenceByte)(0).TransactionNumber = CUShort(SequenceNumber)
        'Requests(SequenceNumber And 255).Responded = False

        Dim d() As MfgControl.AdvancedHMI.Drivers.CLXTag = DLL(MyDLLInstance).GetCLXTags(10, SequenceNumber, MyObjectID)

        Return d
    End Function

    Public Function ReadClock() As Date
        Dim SequenceNumber As UShort = CUShort(DLL(MyDLLInstance).GetNextTransactionNumber(32767))
        Dim SequenceByte As Integer = SequenceNumber And 255
        Requests(SequenceByte) = New List(Of CLXAddressRead)
        Requests(SequenceByte).Add(New AdvancedHMIDrivers.CLXAddressRead)
        Requests(SequenceByte)(0).TransactionNumber = CUShort(SequenceNumber)
        waitHandle(SequenceByte).Reset()

        DLL(MyDLLInstance).GetTime(SequenceNumber, MyObjectID)


        Dim Signalled As Boolean = waitHandle(SequenceByte).WaitOne(2000)


        If Signalled Then
            If Requests(SequenceByte)(0).Response IsNot Nothing Then
                Dim DateOffset As Date = CDate("01/01/1970 00:00:00")
                Dim DateOffsetInt As Int64 = DateOffset.ToBinary
                Dim PLCDateValue As Int64 = BitConverter.ToInt64(Requests(SequenceByte)(0).Response.RawData, 10)
                Dim PLCDate As Date = Date.FromBinary(DateOffsetInt + PLCDateValue * 10)
                Return PLCDate
            End If
        End If
    End Function

    Public Sub SetClock(newTime As Date)
        Dim SequenceNumber As UShort = CUShort(DLL(MyDLLInstance).GetNextTransactionNumber(32767))
        Dim SequenceByte As Integer = SequenceNumber And 255
        Requests(SequenceByte) = New List(Of CLXAddressRead)
        Requests(SequenceByte).Add(New AdvancedHMIDrivers.CLXAddressRead)
        Requests(SequenceByte)(0).TransactionNumber = CUShort(SequenceNumber)
        waitHandle(SequenceByte).Reset()

        DLL(MyDLLInstance).SetTime(newTime, SequenceNumber, MyObjectID)
    End Sub


    '******************************************************************************************************
    '* Write Section
    '******************************************************************************************************

    '******************************************************
    ' Write a single integer value to a PLC data table
    '******************************************************
    Public Function Write(ByVal startAddress As String, ByVal dataToWrite As Integer) As Integer
        Dim temp(1) As String
        temp(0) = Convert.ToString(dataToWrite)
        Return Write(startAddress, 1, temp)
    End Function


    '******************************************************
    ' Write a single floating point value to a data table
    '******************************************************
    Public Function Write(ByVal startAddress As String, ByVal dataToWrite As Single) As Integer
        Dim temp(1) As Single
        temp(0) = dataToWrite
        Return Write(startAddress, 1, temp)
    End Function

    '****************************
    '* Write an array of Singles
    '****************************
    Public Function Write(ByVal startAddress As String, ByVal numberOfElements As Integer, ByVal dataToWrite() As Single) As Integer
        Dim StringVals(numberOfElements) As String
        For i As Integer = 0 To numberOfElements - 1
            StringVals(i) = Convert.ToString(dataToWrite(i))
        Next

        Write(startAddress, numberOfElements, StringVals)
    End Function

    '***********************************
    '* Write all single values in array
    '***********************************
    Public Function Write(ByVal startAddress As String, ByVal dataToWrite() As Single) As Integer
        Dim StringVals(dataToWrite.Length - 1) As String
        For i As Integer = 0 To StringVals.Length - 1
            StringVals(i) = Convert.ToString(dataToWrite(i))
        Next

        Write(startAddress, dataToWrite.Length, StringVals)
    End Function


    '**********************************************
    '* Write specified number of elements in array
    '**********************************************
    Public Function Write(ByVal startAddress As String, ByVal numberOfElements As Integer, ByVal dataToWrite() As Integer) As Integer
        Dim StringVals(numberOfElements) As String
        For i As Integer = 0 To numberOfElements - 1
            StringVals(i) = Convert.ToString(dataToWrite(i))
        Next

        Write(startAddress, numberOfElements, StringVals)
    End Function

    '******************************
    '* Write all integers in array
    '******************************
    Public Function Write(ByVal startAddress As String, ByVal dataToWrite() As Integer) As Integer
        Dim StringVals(dataToWrite.Length - 1) As String
        For i As Integer = 0 To StringVals.Length - 1
            StringVals(i) = Convert.ToString(dataToWrite(i))
        Next

        Write(startAddress, dataToWrite.Length, StringVals)
    End Function

    '***********************
    '* Write a single value
    '***********************
    Public Function Write(ByVal startAddress As String, ByVal dataToWrite As String) As String Implements IComComponent.Write
        If dataToWrite Is Nothing Then
            Return "0"
        End If
        Dim v() As String = {dataToWrite}

        Write(startAddress, 1, v)

        Return "0"
    End Function

    '*******************************
    '* Write to DLL
    '*******************************
    Public Function Write(ByVal startAddress As String, ByVal numberOfElements As Integer, ByVal dataToWrite() As String) As Integer
        Dim StringVals(numberOfElements - 1) As String
        For i As Integer = 0 To numberOfElements - 1
            StringVals(i) = Convert.ToString(dataToWrite(i))
        Next

        'If MyDLLInstance <= 0 Then CreateDLLInstance()
        'TODO: throw exception
        'If MyDLLInstance <= 0 Then Exit Function

        Dim SequenceNumber As Integer = DLL(MyDLLInstance).GetNextTransactionNumber(32767)  ' TNS1.GetNextNumber("WD", MyObjectID)

        Dim TransactionByte As Integer = SequenceNumber And 255

        Requests(TransactionByte) = New List(Of CLXAddressRead)
        Requests(TransactionByte).Add(New CLXAddressRead(startAddress))
        Dim tag As New CLXAddressRead
        tag.TagName = startAddress
        SyncLock (ReadLock)
            DLL(MyDLLInstance).WriteTagValue(tag, StringVals, numberOfElements, SequenceNumber, MyObjectID)
        End SyncLock

        '* Make writing Synchronous to avoid Send Que Full
        Dim signal As Boolean = waitHandle(TransactionByte).WaitOne(2500)
        'Dim result As Integer = WaitForResponse(SequenceNumber, 1000)


        If Requests(TransactionByte)(0).Response IsNot Nothing Then
            If Requests(TransactionByte)(0).Response.ErrorId <> 0 Then
                Throw New MfgControl.AdvancedHMI.Drivers.Common.PLCDriverException(Requests(TransactionByte)(0).Response.ErrorId, Requests(TransactionByte)(0).Response.ErrorMessage)
            End If
        End If

    End Function
    '******************************************************************************************************


    Public Function GetTagInformation(ByVal TagName As String) As MfgControl.AdvancedHMI.Drivers.CLXTag
        Dim Tag As New CLXAddressRead
        Tag.TagName = TagName

        Return DLL(MyDLLInstance).GetTagInformation(Tag, MyObjectID)
    End Function

    Public Shared Function GetByteCount(ByVal AbreviatedType As Byte) As Integer
        '* Get the element size in bytes
        Dim ElementSize As Integer
        Select Case AbreviatedType
            Case &HC1 '* BIT
                ElementSize = 1
            Case &HC2 '* SINT
                ElementSize = 1
            Case &HC3 ' * INT
                ElementSize = 2
            Case &HC4, &HCA '* DINT, REAL Value read (&H91)
                ElementSize = 4
            Case &HC5 ' * LINT
                ElementSize = 8
            Case &HD3 '* Bool Array
                ElementSize = 4
            Case &H82, &H83 ' * Timer, Counter, Control
                ElementSize = 14
            Case &HCE '* String
                'ElementSize = ReturnedData(0) + ReturnedData(1) * 256
                ElementSize = 88
            Case Else
                ElementSize = 2
        End Select

        Return ElementSize
    End Function


    '********************************************************************
    '* Extract the data from the byte stream returned
    '* Use the abreviated type byte that is returned with data
    '********************************************************************
    Private Shared Function ExtractData(ByVal startAddress As String, ByVal AbreviatedType As Byte, ByVal returnedData() As Byte, startIndex As Integer) As String()
        '* Get the element size in bytes
        Dim ElementSize As Integer = GetByteCount(AbreviatedType)
        'Select Case AbreviatedType
        '    Case &HC1 '* BIT
        '        ElementSize = 1
        '    Case &HC2 '* SINT
        '        ElementSize = 1
        '    Case &HC3 ' * INT
        '        ElementSize = 2
        '    Case &HC4, &HCA '* DINT, REAL Value read (&H91)
        '        ElementSize = 4
        '    Case &HC5 ' * LINT
        '        ElementSize = 8
        '    Case &HD3 '* Bool Array
        '        ElementSize = 4
        '    Case &H82, &H83 ' * Timer, Counter, Control
        '        ElementSize = 14
        '    Case &HCE '* String
        '        'ElementSize = ReturnedData(0) + ReturnedData(1) * 256
        '        ElementSize = 88
        '    Case Else
        '        ElementSize = 2
        'End Select

        Dim BitsPerElement As Integer = ElementSize * 8
        '***************************************************
        '* Extract returned data into appropriate data type
        '***************************************************
        Dim ElementsToReturn As Integer = Convert.ToInt32(Math.Floor((returnedData.Length - startIndex) / ElementSize) - 1)
        '* Bit Arrays are return as DINT, so it will have to be extracted
        Dim BitIndex As Integer
        If AbreviatedType = &HD3 Then
            If startAddress.LastIndexOf("[") > 0 Then
                BitIndex = Convert.ToInt32(startAddress.Substring(startAddress.LastIndexOf("[") + 1, startAddress.LastIndexOf("]") - startAddress.LastIndexOf("[") - 1))
            End If
            BitIndex -= Convert.ToInt32(Math.Floor(BitIndex / 32) * 32)
            '* Return all the bits that came back even if less were requested
            ElementsToReturn = (returnedData.Length - startIndex) * 8 - BitIndex - 1
            BitsPerElement = 32
        Else
            '* 18-MAY-12
            '* Check if it is addressing a single bit in a larger data type
            Dim PeriodPos As Integer = startAddress.IndexOf(".")
            If PeriodPos > 0 Then
                Dim SubElement As String = startAddress.Substring(PeriodPos + 1)

                Try
                    If Integer.TryParse(SubElement, BitIndex) Then
                        'BitIndex = Convert.ToInt32(SubElement)

                        Select Case AbreviatedType
                            Case &HC3 '* INT Value read 
                                BitsPerElement = 16
                            Case &HC4 '* DINT Value read (&H91)
                                BitsPerElement = 32
                            Case &HC2 '* SINT
                                BitsPerElement = 8
                        End Select

                        If BitIndex > 0 And BitIndex < BitsPerElement Then
                            '* Force it to appear like a bit array
                            AbreviatedType = &HD3
                            BitIndex -= Convert.ToInt32(Math.Floor(BitIndex / BitsPerElement) * BitsPerElement)
                            '* Return all the bits that came back even if less were requested
                            ElementsToReturn = (returnedData.Length - startIndex) * 8 - BitIndex - 1
                        End If
                    End If
                Catch ex As Exception
                    '* If the value can not be converted, then it is not a valid integer
                End Try
            End If
        End If

        Dim result(ElementsToReturn) As String

        '* If requesting 0 elements, then default to 1
        'Dim ArrayElements As Int16 = Math.Max(result.Length - 1 - 1, 0)


        Select Case AbreviatedType
            Case &HC1 '* BIT
                For i As Integer = 0 To result.Length - 1
                    If returnedData(i + startIndex) > 0 Then
                        result(i) = "True"
                    Else
                        result(i) = "False"
                    End If
                Next
            Case &HCA '* REAL read (&H8A)
                For i As Integer = 0 To result.Length - 1
                    result(i) = Convert.ToString(BitConverter.ToSingle(returnedData, (i * 4 + startIndex)))
                Next
            Case &HC3 '* INT Value read 
                For i As Integer = 0 To result.Length - 1
                    result(i) = Convert.ToString(BitConverter.ToInt16(returnedData, (i * 2) + startIndex))
                Next
            Case &HC4 '* DINT Value read (&H91)
                For i As Integer = 0 To result.Length - 1
                    result(i) = Convert.ToString(BitConverter.ToInt32(returnedData, (i * 4) + startIndex))
                Next
            Case &HC5 '* LINT Value read
                For i As Integer = 0 To result.Length - 1
                    result(i) = Convert.ToString(BitConverter.ToInt64(returnedData, (i * 8) + startIndex))
                Next
            Case &HC2 '* SINT
                For i As Integer = 0 To result.Length - 1
                    '* Ver 3.59
                    If returnedData(i + startIndex) < 128 Then
                        result(i) = Convert.ToString(returnedData(i + startIndex))
                    Else
                        result(i) = Convert.ToString(returnedData(i + startIndex) - 256)
                    End If
                Next
            Case &HD3 '* BOOL Array
                Dim i As Integer
                Dim x, BitValue As UInt64
                Dim CurrentBitPos As Integer = BitIndex
                For j = 0 To ((returnedData.Length - startIndex) / ElementSize) - 1
                    Select Case ElementSize
                        Case 1 '* SINT
                            x = returnedData(Convert.ToInt32(j * ElementSize + startIndex))
                        Case 2 ' * INT
                            x = BitConverter.ToUInt16(returnedData, Convert.ToInt32(j * ElementSize + startIndex))
                        Case 8 ' * LINT
                            x = BitConverter.ToUInt64(returnedData, Convert.ToInt32(j * ElementSize + startIndex))
                        Case Else
                            x = BitConverter.ToUInt32(returnedData, Convert.ToInt32(j * ElementSize + startIndex))
                    End Select

                    'x = BitConverter.ToUInt32(returnedData, Convert.ToInt32(j * ElementSize + startIndex))
                    While CurrentBitPos < BitsPerElement
                        BitValue = Convert.ToUInt64(2 ^ (CurrentBitPos))
                        result(i) = Convert.ToString((x And BitValue) > 0)
                        i += 1
                        CurrentBitPos += 1
                    End While
                    CurrentBitPos = 0
                Next
            Case &H82, &H83 '* TODO: Timer, Counter, Control 
                Dim StartByte As Int16 = 2
                '                Dim x = startAddress
                '* Look for which sub element is specificed
                If startAddress.IndexOf(".") >= 0 Then
                    If startAddress.ToUpper.IndexOf("PRE") > 0 Then
                        StartByte = 6
                    ElseIf startAddress.ToUpper.IndexOf("ACC") > 0 Then
                        StartByte = 10
                    End If
                Else
                    '* If no subelement, then use ACC
                End If

                For i As Integer = 0 To result.Length - 1
                    result(i) = Convert.ToString(BitConverter.ToInt32(returnedData, (i + StartByte + startIndex)))
                Next
            Case &HCE ' * String
                For i As Integer = 0 To result.Length - 1
                    result(i) = System.Text.Encoding.ASCII.GetString(returnedData, 88 * i + 4 + startIndex, returnedData(88 * i + startIndex) + returnedData(88 * i + 1 + startIndex) * 256)
                Next
            Case Else
                For i As Integer = 0 To result.Length - 1
                    result(i) = Convert.ToString(BitConverter.ToInt16(returnedData, (i * 2) + startIndex))
                Next
        End Select


        Return result
    End Function
#End Region

#Region "Subscriptions"
    Private GroupedSubscriptionReads As New System.Collections.Concurrent.ConcurrentDictionary(Of Integer, SubscriptionRead)
    Private SubscriptionList As New List(Of SubscriptionInfo)

    Private CurrentSubscriptionID As Integer = 1
    Private SubscriptionListChanged As Boolean

    Private SubscriptionThread As System.Threading.Thread

    Private Class SubscriptionInfo
        Public Sub New()
            PLCAddress = New MfgControl.AdvancedHMI.Drivers.CLXAddress
        End Sub

        Public PLCAddress As MfgControl.AdvancedHMI.Drivers.CLXAddress

        Public dlgCallBack As EventHandler(Of MfgControl.AdvancedHMI.Drivers.Common.PlcComEventArgs)
        Public PollRate As Integer
        Public ID As Integer
        ' Public ElementsToRead As Integer
        'Public SkipReads As Integer
        Public DataType As Byte
    End Class

    '* This is used to optimize the reads of the subscriptions
    Private Class SubscriptionRead
        Implements ICloneable

        'Friend TagName As List(Of String)
        Friend CLXAddress As List(Of CLXAddressRead)
        'Friend NumberToRead As List(Of Integer)

        Public Function Clone() As Object Implements ICloneable.Clone
            Dim x As New SubscriptionRead
            x.CLXAddress = New List(Of CLXAddressRead)
            For i = 0 To Me.CLXAddress.Count - 1
                x.CLXAddress.Add(DirectCast(Me.CLXAddress(i).Clone, CLXAddressRead))
            Next

            'For i = 0 To Me.NumberToRead.Count - 1
            '    x.NumberToRead = Me.NumberToRead
            'Next

            Return x
        End Function
    End Class


    Public Function Subscribe(ByVal PLCAddress As String, ByVal numberOfElements As Int16, ByVal PollRate As Integer, ByVal CallBack As EventHandler(Of MfgControl.AdvancedHMI.Drivers.Common.PlcComEventArgs)) As Integer Implements IComComponent.Subscribe
        If m_DisableSubscriptions Then
            Throw New MfgControl.AdvancedHMI.Drivers.Common.PLCDriverException("Cannot create new subscription when DisableSubscriptions=True")
        End If

        If m_PollRateOverride > 0 Then
            PollRate = m_PollRateOverride
        End If

        '* Avoid a 0 poll rate
        If PollRate <= 0 Then
            PollRate = 500
        End If


        '* Valid address?
        'If ParsedResult.FileType <> 0 Then
        Dim tmpPA As New SubscriptionInfo
        tmpPA.PLCAddress.TagName = PLCAddress
        tmpPA.PollRate = PollRate
        tmpPA.dlgCallBack = CallBack

        ''* Attempt to read this value in order to get the DataType
        'SyncLock (ReadLock)
        '    Try
        '        Read(PLCAddress)
        '    Catch ex As Exception
        '        Dim x As New MfgControl.AdvancedHMI.Drivers.Common.PlcComEventArgs(-50, ex.Message)
        '        x.PlcAddress = PLCAddress
        '        'OnComError(x)
        '        ComErrorHandler(Me, x)
        '    End Try
        'End SyncLock


        tmpPA.ID = CurrentSubscriptionID
        '* The ID is used as a reference for removing polled addresses
        CurrentSubscriptionID += 1

        'tmpPA.ElementsToRead = numberOfElements
        tmpPA.PLCAddress.NumberOfElements = numberOfElements

        SyncLock (SubscriptionLock)
            SubscriptionList.Add(tmpPA)

            '* Sort the list to allow easier grouping
            SubscriptionList.Sort(AddressOf SortPolledAddresses)

            '* Flag this so it will run the optimizer after the first read
            SubscriptionListChanged = True
        End SyncLock
        '* Put it in the read list. Later it will get grouped for optimizing
        'Dim x As New SubscriptionRead
        'x.TagName = tmpPA.PLCAddress.TagName
        'x.NumberToRead = tmpPA.ElementsToRead
        'SyncLock (CollectionLock)
        '    GroupedSubscriptionReads.TryAdd(GroupedSubscriptionReads.Count, x)
        'End SyncLock


        '* Create an list of optimized reads
        'CreateGroupedReadList()

        '* Start the subscription updater if not already running
        If SubscriptionThread Is Nothing Then
            SubscriptionThread = New System.Threading.Thread(AddressOf SubscriptionUpdate)
            SubscriptionThread.Name = "SubscriptionThread"
            SubscriptionThread.IsBackground = True
            SubscriptionThread.Start()
            'AddHandler SubscriptionThread.DoWork, AddressOf SubscriptionUpdate
            'SubscriptionThread.RunWorkerAsync()
        End If

        Return tmpPA.ID
    End Function

    '***************************************************************
    '* Used to sort polled addresses by File Number and element
    '* This helps in optimizing reading
    '**************************************************************
    Private Function SortPolledAddresses(ByVal A1 As SubscriptionInfo, ByVal A2 As SubscriptionInfo) As Integer
        '* Sort Tags

        '* Arrays always go to the top of the list so grouped multi service reads work properly
        If (A1.PLCAddress.ArrayIndex1 < 0 And A2.PLCAddress.ArrayIndex1 >= 0) Then
            Return -1
        End If
        If (A1.PLCAddress.ArrayIndex1 >= 0 And A2.PLCAddress.ArrayIndex1 < 0) Then
            Return 1
        End If

        '* 22-JAN-13
        '* Are they in the same array?
        If A1.PLCAddress.BaseArrayTag <> A2.PLCAddress.BaseArrayTag Then
            If A1.PLCAddress.BaseArrayTag > A2.PLCAddress.BaseArrayTag Then
                Return 1
            ElseIf A1.PLCAddress.BaseArrayTag < A2.PLCAddress.BaseArrayTag Then
                Return -1
            End If
        Else
            '* TODO : sort multidimensional array
            If A1.PLCAddress.ArrayIndex1 > A2.PLCAddress.ArrayIndex1 Then
                Return 1
            ElseIf A1.PLCAddress.ArrayIndex1 < A2.PLCAddress.ArrayIndex1 Then
                Return -1
            ElseIf A1.PLCAddress.BitNumber >= 0 And A2.PLCAddress.BitNumber >= 0 Then
                If A1.PLCAddress.BitNumber > A2.PLCAddress.BitNumber Then
                    Return 1
                ElseIf A1.PLCAddress.BitNumber < A2.PLCAddress.BitNumber Then
                    Return -1
                End If
            End If
        End If

        Return 0
    End Function

    Public Function UnSubscribe(ByVal ID As Integer) As Integer Implements IComComponent.Unsubscribe
        Dim i As Integer = 0
        While i < SubscriptionList.Count AndAlso SubscriptionList(i).ID <> ID
            i += 1
        End While

        If i < SubscriptionList.Count Then
            SubscriptionList.RemoveAt(i)
        End If

        ' SyncLock (CollectionLock)
        'CreateGroupedReadList()
        'End SyncLock

        SubscriptionListChanged = True

        Return 0
    End Function

    '******************************************************************************************
    '* Group together subscriptions for fewer reads to increase speed and efficiency
    '******************************************************************************************
    Private Sub CreateGroupedReadList()
        SyncLock (SubscriptionLock)
            GroupedSubscriptionReads.Clear()

            SubscriptionListChanged = False

            Dim i, FirstElement As Integer

            Dim g As List(Of CLXAddressRead) = GroupArrayElements()

            While i < g.Count
                '    '* Add this read to the list  of grouped reads
                Dim x As New SubscriptionRead
                x.CLXAddress = New List(Of CLXAddressRead)
                x.CLXAddress.Add(g(i))
                FirstElement = i
                i += 1
                '* Add additional Items for multi service read?
                If Not m_DisableMultiServiceRequest Then
                    If i < g.Count AndAlso g(FirstElement).AbreviatedDataType > 0 Then   'AndAlso NumberToRead = 1 And GroupedCount = 1 Then
                        'If g(FirstElement + 1).NumberOfElements = 1 Then
                        Dim TotalNameLength As Integer = g(FirstElement).TagName.Length
                        Dim TotalReadBytes As Integer = CInt(GetByteCount(CByte(g(FirstElement).AbreviatedDataType And 255)) * g(FirstElement).NumberOfElements) + 7
                        While i < g.Count AndAlso _
                                (TotalNameLength + (g(i).TagName.Length + 7)) < (m_CIPConnectionSize - 100) AndAlso _
                                (TotalReadBytes + CInt(GetByteCount(CByte(g(i).AbreviatedDataType And 255)) * g(i).NumberOfElements) + 6) < (m_CIPConnectionSize - 100) AndAlso _
                                g(FirstElement).AbreviatedDataType > 0 AndAlso _
                                g(i).AbreviatedDataType > 0
                            '    SubscriptionList(FirstElement).PLCAddress.BitNumber < 0 AndAlso _
                            '* Make sure this is not the same tag and element count
                            If String.Compare(g(FirstElement).TagName, g(i).TagName, True) <> 0 Then
                                x.CLXAddress.Add(New CLXAddressRead(g(i).TagName))
                                x.CLXAddress(x.CLXAddress.Count - 1).NumberOfElements = g(i).NumberOfElements
                                TotalNameLength += (g(i).TagName.Length + 7)
                                TotalReadBytes += CInt(GetByteCount(CByte(g(i).AbreviatedDataType And 255)) * g(i).NumberOfElements) + 7
                            End If
                            i += 1
                        End While
                        'End If
                    End If
                End If

                GroupedSubscriptionReads.TryAdd(GroupedSubscriptionReads.Count, x)

                ' i += GroupedCount
            End While
        End SyncLock
    End Sub

    '****************************************************************************************************
    '* This function groups all of the individual subscribed elemets in the same array into single reads
    '* It is intended to optimize the necessary reads to as few as possible
    '****************************************************************************************************
    Private Function GroupArrayElements() As List(Of CLXAddressRead)
        Dim i, NumberToRead, FirstElement As Integer
        Dim GroupedItems As New List(Of CLXAddressRead)

        While i < SubscriptionList.Count
            NumberToRead = SubscriptionList(i).PLCAddress.NumberOfElements
            FirstElement = i
            Dim GroupedCount As Integer = 1

            '* Optimize by reading array elements together - only single dimension array and basic data types. Do not group
            If SubscriptionList(FirstElement).PLCAddress.ArrayIndex1 >= 0 And SubscriptionList(FirstElement).PLCAddress.ArrayIndex2 < 0 And _
                SubscriptionList(FirstElement).DataType >= &HC2 And SubscriptionList(FirstElement).DataType <= &HD3 And
                (SubscriptionList(FirstElement).PLCAddress.SubElement = "") Then
                Try
                    While FirstElement + GroupedCount < SubscriptionList.Count AndAlso _
                      SubscriptionList(FirstElement).PLCAddress.BaseArrayTag = SubscriptionList(FirstElement + GroupedCount).PLCAddress.BaseArrayTag AndAlso
                       (SubscriptionList(FirstElement + GroupedCount).PLCAddress.ArrayIndex1 - SubscriptionList(FirstElement).PLCAddress.ArrayIndex1) < 50

                        '* Add the number of span between the array
                        '* check to make sure the next element is higher
                        NumberToRead = Math.Max(NumberToRead, SubscriptionList(FirstElement + GroupedCount).PLCAddress.ArrayIndex1 - SubscriptionList(FirstElement).PLCAddress.ArrayIndex1 + 1)
                        GroupedCount += 1
                    End While
                Catch ex As Exception
                    Dim dbg = 0
                End Try
            End If

            '* Strip off the bit because we will read the complete element, then extract the bit after data is returned
            '* A BOOL array (&HD3) does not have a bit number
            Dim TagNameNoBit As String = SubscriptionList(FirstElement).PLCAddress.TagName
            If SubscriptionList(FirstElement).PLCAddress.BitNumber >= 0 And SubscriptionList(FirstElement).DataType <> &HD3 Then
                TagNameNoBit = TagNameNoBit.Substring(0, TagNameNoBit.LastIndexOf("."))
            End If

            '* Add this read to the list  of grouped reads
            Dim x As New SubscriptionRead
            x.CLXAddress = New List(Of CLXAddressRead)


            '* If it is a BOOL Array round down to the nearest DINT aligned bit
            If SubscriptionList(FirstElement).DataType = &HD3 Then
                Dim DINTNumber As Integer '  = Convert.ToInt32(Math.Floor(SubscriptionList(FirstElement).PLCAddress.ArrayIndex1 / 32))
                '* CLX Address treats this as a DINT array, so ArrayIndex is the word
                DINTNumber = SubscriptionList(FirstElement).PLCAddress.ArrayIndex1
                x.CLXAddress.Add(New CLXAddressRead(TagNameNoBit.Substring(0, TagNameNoBit.LastIndexOf("[") + 1) & (DINTNumber * 32) & "]"))
                NumberToRead *= 32
            End If

            Dim a As New CLXAddressRead
            a.TagName = TagNameNoBit
            a.NumberOfElements = NumberToRead
            a.AbreviatedDataType = SubscriptionList(FirstElement).DataType
            GroupedItems.Add(a)

            i += GroupedCount
        End While

        Return GroupedItems
    End Function

    '**************************************************************
    '* Perform the reads for the variables added for notification
    '* Attempt to optimize by grouping reads
    '**************************************************************
    'Dim sw1 As New Stopwatch
    Private StopSubscriptions As Boolean
    Private ReadTime As New Stopwatch
    Private Sub SubscriptionUpdate()   '(sender As System.Object, e As System.ComponentModel.DoWorkEventArgs)
        While Not StopSubscriptions
            'Dim index As Integer
            If Not m_DisableSubscriptions And GroupedSubscriptionReads IsNot Nothing Then
                '* 3-JUN-13 Do not read data until handles are created to avoid exceptions
                If m_SynchronizingObject Is Nothing OrElse DirectCast(m_SynchronizingObject, Windows.Forms.Control).IsHandleCreated Then
                    Dim DelayBetweenPackets As Integer
                    'Dim response As Integer = 1
                    Dim TransactionNumber As Integer
                    Dim TransactionByte As Integer
                    Dim Signalled As Boolean
                    'Dim T1, T2, T3, T4 As Long
                    'index = 0
                    For Each key In GroupedSubscriptionReads.Keys
                        'While index < GroupedSubscriptionReads.Count And Not StopSubscriptions
                        '* Evenly space out read requests to avoid Send Que Full
                        DelayBetweenPackets = Convert.ToInt32(Math.Max(0, Math.Floor(m_PollRateOverride / GroupedSubscriptionReads.Count)))
                        ReadTime.Reset()
                        ReadTime.Start()
                        Try
                            If Not m_DisableSubscriptions And Not StopSubscriptions Then
                                If GroupedSubscriptionReads(key).CLXAddress.Count > 1 Then
                                    '* multiple individual items
                                    Dim ReadItems As New List(Of AdvancedHMIDrivers.CLXAddressRead)
                                    For index = 0 To GroupedSubscriptionReads(key).CLXAddress.Count - 1
                                        'Dim a1 As New AdvancedHMIDrivers.CLXAddressRead
                                        'a1.TagName = GroupedSubscriptionReads(key).TagName(index)
                                        GroupedSubscriptionReads(key).CLXAddress(index).Response = Nothing
                                        ReadItems.Add(DirectCast(GroupedSubscriptionReads(key).CLXAddress(index).Clone, AdvancedHMIDrivers.CLXAddressRead))
                                    Next

                                    TransactionNumber = Me.BeginReadMultiple(ReadItems)
                                Else
                                    '* An array or single item
                                    TransactionNumber = Me.BeginRead(GroupedSubscriptionReads(key).CLXAddress(0), GroupedSubscriptionReads(key).CLXAddress(0).NumberOfElements)
                                End If


                                TransactionByte = TransactionNumber And 255
                                'T2 = ReadTime.ElapsedMilliseconds
                                Signalled = waitHandle(TransactionByte).WaitOne(3500 + CInt(GroupedSubscriptionReads(key).CLXAddress(0).NumberOfElements / 5))
                                'T3 = ReadTime.ElapsedMilliseconds

                                If Signalled Then
                                    'For ind = 0 To Requests(TransactionByte).Count - 1
                                    'waitHandle(TransactionByte).Reset()
                                    Try
                                        If Requests(TransactionByte)(0).Response IsNot Nothing Then
                                            SendToSubscriptions(Requests(TransactionByte)(0).Response)
                                        Else
                                            Dim dbg = 0
                                        End If
                                    Catch ex As Exception
                                        Throw
                                    End Try
                                    'Next
                                End If

                            End If
                        Catch ex As MfgControl.AdvancedHMI.Drivers.Common.PLCDriverException
                            Dim x As New MfgControl.AdvancedHMI.Drivers.Common.PlcComEventArgs(ex.ErrorCode, ex.Message)
                            Try
                                SendToSubscriptions(x)
                            Catch ex1 As Exception
                                Dim dbg = 0
                            End Try
                            Threading.Thread.Sleep(m_PollRateOverride)
                        Catch ex As Exception
                            '* Send this message back to the requesting control
                            Dim x As New MfgControl.AdvancedHMI.Drivers.Common.PlcComEventArgs(-99, ex.Message)
                            SendToSubscriptions(x)
                            Threading.Thread.Sleep(m_PollRateOverride)
                        End Try

                        'T4 = ReadTime.ElapsedMilliseconds
                        'Console.WriteLine("SU=" & T2 & "," & T3 & "," & T4)


                        '* Evenly space out the reads to avoid SendQue Full
                        ReadTime.Stop()
                        If Convert.ToInt32(ReadTime.ElapsedMilliseconds) < DelayBetweenPackets Then
                            Threading.Thread.Sleep(DelayBetweenPackets - Convert.ToInt32(ReadTime.ElapsedMilliseconds))
                        End If
                        'index += 1
                        'End While
                    Next
                End If
            End If

            If GroupedSubscriptionReads.Count <= 0 Or m_DisableSubscriptions Then
                Threading.Thread.Sleep(m_PollRateOverride)
            End If

            If SubscriptionListChanged Then
                'SyncLock (CollectionLock)
                CreateGroupedReadList()
                'End SyncLock
            End If
            'Console.WriteLine("SUE=" & ReadTime.ElapsedMilliseconds)
        End While


    End Sub

    Private Sub SendToSubscriptions(ByVal e As MfgControl.AdvancedHMI.Drivers.Common.PlcComEventArgs)
        Dim TNSByte As Integer = e.TransactionNumber And 255

        If (Requests(TNSByte) Is Nothing) OrElse (e.Values.Count <= 0 And e.ErrorId = 0) Then
            Exit Sub
        End If

        '*********************************************************
        '* Check to see if this is from the Polled variable list
        '*********************************************************
        Dim i As Integer
        For index = 0 To Requests(TNSByte).Count - 1
            i = 0
            While i < SubscriptionList.Count
                '* Check length first for performance gain, compare is intensive operation
                If (SubscriptionList(i).PLCAddress.BaseArrayTag.Length = Requests(TNSByte)(index).BaseArrayTag.Length) AndAlso _
                     String.Compare(SubscriptionList(i).PLCAddress.BaseArrayTag, Requests(TNSByte)(index).BaseArrayTag, True) = 0 Then
                    If e.ErrorId = 0 Then
                        Dim ElementNumber1 As Integer = Requests(TNSByte)(index).ArrayIndex1
                        Dim ElementNumber2 As Integer = Requests(TNSByte)(index).ArrayIndex2
                        Dim ElementNumber3 As Integer = Requests(TNSByte)(index).ArrayIndex3

                        Dim FirstElementRead As Integer = ElementNumber1
                        Dim LastElementRead As Integer = FirstElementRead + Requests(TNSByte)(index).Response.Values.Count - 1
                        'Dim HighestElementInArrayNeeded As Integer = (SubscriptionList(i).PLCAddress.ArrayIndex1 - Requests(TNSByte)(index).ArrayIndex1) + 1

                        Dim MaxElementCountRead As Integer = Requests(TNSByte)(index).Response.Values.Count
                        'Dim FirstElementNumberThatWasRead As Integer = Requests(TNSByte).ArrayIndex1
                        Dim FirstElementInArrayToUse As Integer = SubscriptionList(i).PLCAddress.ArrayIndex1 - Requests(TNSByte)(index).ArrayIndex1

                        '* Boolean array are read like DINT
                        If Requests(TNSByte)(index).AbreviatedDataType = &HD3 Then
                            'HighestElementInArrayNeeded = SubscriptionList(i).PLCAddress.ArrayIndex1 * 32 - Requests(TNSByte)(index).ArrayIndex1 * 32 + SubscriptionList(i).PLCAddress.BitNumber + SubscriptionList(i).PLCAddress.NumberOfElements - 1
                            LastElementRead = SubscriptionList(i).PLCAddress.ArrayIndex1 * 32 - Requests(TNSByte)(index).ArrayIndex1 * 32 + SubscriptionList(i).PLCAddress.BitNumber + SubscriptionList(i).PLCAddress.NumberOfElements - 1
                            FirstElementInArrayToUse = SubscriptionList(i).PLCAddress.ArrayIndex1 * 32 + SubscriptionList(i).PLCAddress.BitNumber - Requests(TNSByte)(index).ArrayIndex1 * 32
                        End If

                        If ElementNumber1 < 0 Then
                            ElementNumber1 = 0
                        End If

                        'If (SubscriptionList(i).PLCAddress.BaseArrayTag = Requests(TNSByte)(index).BaseArrayTag And _
                        'If ((FirstElementInArrayToUse >= 0 And HighestElementInArrayNeeded <= MaxElementCountRead) Or SubscriptionList(i).PLCAddress.ArrayIndex1 < 0) Then
                        If (SubscriptionList(i).PLCAddress.ArrayIndex1 >= FirstElementRead AndAlso _
                            (SubscriptionList(i).PLCAddress.ArrayIndex1 + SubscriptionList(i).PLCAddress.NumberOfElements - 1) <= LastElementRead) Or _
                                  (SubscriptionList(i).PLCAddress.ArrayIndex1 < 0) Then

                            Dim BitResult(SubscriptionList(i).PLCAddress.NumberOfElements - 1) As String

                            '* a -1 in ArrayElement number means it is not an array
                            If SubscriptionList(i).PLCAddress.ArrayIndex1 >= 0 Then               ' And (Requests(TNSByte).AbreviatedDataType <> &HD3) Then
                                '* All other data types
                                For k As Integer = 0 To SubscriptionList(i).PLCAddress.NumberOfElements - 1
                                    BitResult(k) = Requests(TNSByte)(index).Response.Values(FirstElementInArrayToUse + k)
                                Next
                            Else
                                BitResult(0) = Requests(TNSByte)(index).Response.Values(0)  ' e.Values(0)
                            End If


                            '* 23-APR-13 Did we read an array of integers, but the subscribed element was a bit?
                            If Requests(TNSByte)(index).BitNumber < 0 And SubscriptionList(i).PLCAddress.BitNumber >= 0 Then
                                '* Had to change Convert.ToInt32 to CInt because it would fail on a decimal
                                BitResult(0) = Convert.ToString((Convert.ToUInt32(2 ^ SubscriptionList(i).PLCAddress.BitNumber) And CInt(BitResult(0))) > 0)
                            End If

                            '* Do we have a subelement?
                            If SubscriptionList(i).PLCAddress.SubElement = "" OrElse SubscriptionList(i).PLCAddress.SubElement = Requests(TNSByte)(index).SubElement Then

                                '* Is it a multi-dimensional array?
                                If Requests(TNSByte)(index).ArrayIndex2 = SubscriptionList(i).PLCAddress.ArrayIndex2 And Requests(TNSByte)(index).ArrayIndex3 = SubscriptionList(i).PLCAddress.ArrayIndex3 Then
                                    '* Make sure the Handle is created on the form
                                    '* 27-AUG-13 This prevents a problem caused by the DataSubscriber
                                    If m_SynchronizingObject Is Nothing OrElse DirectCast(m_SynchronizingObject, Windows.Forms.Control).IsHandleCreated Then
                                        Dim f As New MfgControl.AdvancedHMI.Drivers.Common.PlcComEventArgs(e.RawData, SubscriptionList(i).PLCAddress.TagName, e.TransactionNumber)

                                        '* Modify the grouped read to be as if it only read this one subscription
                                        f.PlcAddress = SubscriptionList(i).PLCAddress.TagName

                                        f.Values = New System.Collections.ObjectModel.Collection(Of String)
                                        For index2 = 0 To BitResult.Length - 1
                                            f.Values.Add(BitResult(index2))
                                        Next

                                        f.SubscriptionID = SubscriptionList(i).ID
                                        'Dim x As New MfgControl.AdvancedHMI.Drivers.Common.PlcComEventArgs(BitResult, SubscriptionList(i).PLCAddress.TagName, e.TransactionNumber)
                                        'x.SubscriptionID = SubscriptionList(i).ID
                                        Dim z() As Object = {Me, f}
                                        '* 27-AUG-14 version 3.67
                                        If m_SynchronizingObject IsNot Nothing AndAlso m_SynchronizingObject.InvokeRequired Then
                                            m_SynchronizingObject.BeginInvoke(SubscriptionList(i).dlgCallBack, z)
                                        Else
                                            SubscriptionList(i).dlgCallBack(Me, f)
                                        End If
                                    End If
                                End If
                            End If
                        End If
                    Else
                        '* Error to send
                        If (SubscriptionList(i).PLCAddress.BaseArrayTag = Requests(TNSByte)(index).BaseArrayTag) Then
                            If m_SynchronizingObject IsNot Nothing Then
                                Dim z() As Object = {Me, e}
                                m_SynchronizingObject.BeginInvoke(SubscriptionList(i).dlgCallBack, z)
                            Else
                                SubscriptionList(i).dlgCallBack(Me, e)
                            End If
                        End If
                    End If
                End If
                i += 1
            End While
        Next
    End Sub
#End Region

#Region "Helper"
    ''****************************************************
    ''* Wait for a response from PLC before returning
    ''****************************************************
    'Private Function WaitForResponse(ByVal rTNS As Integer, timeout As Integer) As Integer
    '    Dim Loops As Integer
    '    While Not Requests(rTNS And 255)(0).Responded And Loops < timeout
    '        System.Threading.Thread.Sleep(1)
    '        Loops += 1
    '    End While

    '    If Loops >= timeout Then
    '        Return -20
    '    Else
    '        Return 0
    '    End If
    'End Function




#End Region

#Region "Events"
    '***************************************************************************************
    '* If an error comes back from the driver, return the description back to the control
    '***************************************************************************************
    Protected Sub DataLinkLayerComError(ByVal sender As Object, ByVal e As MfgControl.AdvancedHMI.Drivers.Common.PlcComEventArgs)
        Dim d() As String = {MfgControl.AdvancedHMI.Drivers.CIPUtilities.DecodeMessage(e.ErrorId)}


        If Requests(e.TransactionNumber And 255) IsNot Nothing AndAlso Requests(e.TransactionNumber And 255).Count > 0 Then
            Requests(e.TransactionNumber And 255)(0).Response = e

            'Requests(e.TransactionNumber And 255)(0).Responded = True
            If waitHandle(e.TransactionNumber And 255) IsNot Nothing Then
                waitHandle(e.TransactionNumber And 255).Set()
            End If
        End If

        OnComError(e)

        SendToSubscriptions(e)
    End Sub


    '********************************************************************************************************************************
    Protected Sub CIPConnectionClosed(ByVal sender As Object, e As EventArgs)
        OnConnectionClosed(e)
    End Sub

    Protected Overridable Sub OnConnectionClosed(ByVal e As EventArgs)
        If m_SynchronizingObject IsNot Nothing Then
            Dim Parameters() As Object = {Me, EventArgs.Empty}
            If m_SynchronizingObject IsNot Nothing Then
                Try
                    m_SynchronizingObject.BeginInvoke(occd, Parameters)
                Catch ex As Exception
                End Try
            End If
        Else
            RaiseEvent ConnectionClosed(Me, e)
        End If
    End Sub

    Dim occd As EventHandler(Of EventArgs) = AddressOf ConnectionClosedSync
    Private Sub ConnectionClosedSync(ByVal sender As Object, ByVal e As EventArgs)
        RaiseEvent ConnectionClosed(Me, e)
    End Sub
    '********************************************************************************************************************************



    Protected Sub CIPConnectionEstablished(ByVal sender As Object, e As EventArgs)
        OnConnectionEstablished(e)
    End Sub

    Protected Overridable Sub OnConnectionEstablished(ByVal e As EventArgs)
        If m_SynchronizingObject IsNot Nothing AndAlso m_SynchronizingObject.InvokeRequired Then
            Dim Parameters() As Object = {Me, EventArgs.Empty}
            If m_SynchronizingObject IsNot Nothing Then
                If TypeOf (m_SynchronizingObject) Is System.Windows.Forms.Control AndAlso DirectCast(m_SynchronizingObject, System.Windows.Forms.Control).IsHandleCreated Then
                    m_SynchronizingObject.BeginInvoke(oced, Parameters)
                End If
            End If
        Else
            RaiseEvent ConnectionEstablished(Me, e)
        End If
    End Sub

    Dim oced As EventHandler(Of EventArgs) = AddressOf ConnectionEstablishedSync
    Private Sub ConnectionEstablishedSync(ByVal sender As Object, ByVal e As EventArgs)
        RaiseEvent ConnectionEstablished(Me, e)
    End Sub



    Protected Sub DataLinkLayerDataReceived(ByVal sender As Object, ByVal e As MfgControl.AdvancedHMI.Drivers.Common.PlcComEventArgs)
        If (e IsNot Nothing AndAlso e.OwnerObjectID = MyObjectID AndAlso Requests(e.TransactionNumber And 255) IsNot Nothing) Then
            '* Check the status byte
            Dim StatusByte As Integer = e.ErrorId
            Dim TransactionByte As Integer = e.TransactionNumber And 255
            ' Console.WriteLine("DR1=" & ReadTime.ElapsedMilliseconds)

            If StatusByte = 6 Then
                If e.RawData IsNot Nothing Then
                    If e.RawData.Length > 2 Then
                        StatusByte = e.RawData(2)
                    End If
                    '* Extended status code, Page 13 of Logix5000 Data Access
                    If StatusByte = &HFF And e.RawData.Length > 5 Then
                        StatusByte = e.RawData(5) * 256 + e.RawData(4)
                    End If
                End If
            End If

            Dim CurrentRequestIndex As Integer
            While CurrentRequestIndex < (Requests(TransactionByte).Count) AndAlso Requests(TransactionByte)(CurrentRequestIndex).Response IsNot Nothing
                'While Requests(TransactionByte)(CurrentRequestIndex).Response IsNot Nothing
                CurrentRequestIndex += 1
            End While

            If CurrentRequestIndex >= Requests(TransactionByte).Count Then
                Exit Sub
            End If

            If StatusByte = 0 Then
                '* Byte 0 is the Service code &H80
                If e.RawData.Length > 0 AndAlso e.RawData(0) = &H83 Then
                    '* Service code 3 is read attributes - Only used in this class to read the clock
                    '* Is the general status=0?
                    If e.RawData(2) = 0 And e.RawData(3) = 0 Then
                        '* Did we get enough data for a date?
                        If e.RawData.Length >= 18 Then
                            Requests(TransactionByte)(CurrentRequestIndex).Response = e
                        End If
                    End If
                Else
                    '**************************************************************
                    '* Only extract and send back if this response contained data
                    '**************************************************************
                    If e.RawData.Length > 5 Then
                        '***************************************************
                        '* Extract returned data into appropriate data type
                        '* Transfer block of data read to the data table array
                        '***************************************************
                        '* TODO: Check array bounds
                        '* Pass the abreviated data type (page 11 of 1756-RM005A)
                        Dim DataType As Byte = e.RawData(4)


                        Dim DataStartIndex As UInt16 = 6
                        '* Is it a complex data type
                        If DataType = &HA0 Then
                            DataType = e.RawData(6)
                            DataStartIndex = 8
                        End If


                        e.PlcAddress = Requests(TransactionByte)(CurrentRequestIndex).TagName
                        Requests(TransactionByte)(CurrentRequestIndex).AbreviatedDataType = DataType

                        '* See if any subscriptions need their DataType set
                        For Each S In SubscriptionList
                            If S.DataType = 0 And String.Compare(S.PLCAddress.BaseArrayTag, Requests(TransactionByte)(CurrentRequestIndex).BaseArrayTag) = 0 Then
                                S.DataType = DataType
                                S.PLCAddress.AbreviatedDataType = DataType
                                SubscriptionListChanged = True
                            End If
                        Next

                        If Requests(TransactionByte) IsNot Nothing AndAlso Requests(TransactionByte)(CurrentRequestIndex).AbreviatedDataType = 0 Then
                            Requests(TransactionByte)(CurrentRequestIndex).AbreviatedDataType = DataType
                        End If

                        Dim d() As String
                        d = ExtractData(Requests(TransactionByte)(CurrentRequestIndex).TagName, DataType, e.RawData, DataStartIndex)

                        Try
                            Requests(TransactionByte)(CurrentRequestIndex).Response = e
                            For Each v In d
                                Requests(TransactionByte)(CurrentRequestIndex).Response.Values.Add(v)
                            Next
                        Catch ex As Exception
                            Dim dbg = 0
                        End Try

                        OnDataReceived(Requests(TransactionByte)(CurrentRequestIndex).Response)
                    End If
                End If
            Else
                '*****************************
                '* Failed Status was returned
                '*****************************
                e.ErrorId = StatusByte
                Requests(TransactionByte)(CurrentRequestIndex).Response = New MfgControl.AdvancedHMI.Drivers.Common.PlcComEventArgs(StatusByte, _
                                                                MfgControl.AdvancedHMI.Drivers.CIPUtilities.DecodeMessage(StatusByte), e.TransactionNumber)

                OnComError(e)
            End If

            'If Requests(TransactionByte)(0) IsNot Nothing Then
            '    'Requests(TransactionByte)(0).Responded = True
            '    'If waitHandle(TransactionByte) IsNot Nothing Then
            '    'End If
            'Else
            '    Dim dbg = 0
            'End If
            If CurrentRequestIndex >= (Requests(TransactionByte).Count - 1) Then
                waitHandle(TransactionByte).Set()
            End If
        End If
    End Sub


    '******************************************************************
    '* This is called when a message instruction was sent from the PLC
    '******************************************************************
    Private Sub DataLink1_UnsolictedMessageRcvd()
        If m_SynchronizingObject IsNot Nothing Then
            Dim Parameters() As Object = {Me, EventArgs.Empty}
            m_SynchronizingObject.BeginInvoke(drsd, Parameters)
        Else
            RaiseEvent UnsolictedMessageRcvd(Me, System.EventArgs.Empty)
        End If
    End Sub

    Protected Overridable Sub OnDataReceived(ByVal e As MfgControl.AdvancedHMI.Drivers.Common.PlcComEventArgs)
        If e Is Nothing Then
            Dim dbg = 0
        End If
        If m_SynchronizingObject IsNot Nothing AndAlso m_SynchronizingObject.InvokeRequired Then
            m_SynchronizingObject.BeginInvoke(drsd, New Object() {Me, e})
        Else
            RaiseEvent DataReceived(Me, e)
        End If
    End Sub

    '****************************************************************************
    '* This is required to sync the event back to the parent form's main thread
    '****************************************************************************
    Private drsd As EventHandler(Of MfgControl.AdvancedHMI.Drivers.Common.PlcComEventArgs) = AddressOf DataReceivedSync
    Private Sub DataReceivedSync(ByVal sender As Object, ByVal e As MfgControl.AdvancedHMI.Drivers.Common.PlcComEventArgs)
        RaiseEvent DataReceived(Me, e)
    End Sub


    '***********************************************************************************************************
    Protected Overridable Sub OnComError(ByVal e As MfgControl.AdvancedHMI.Drivers.Common.PlcComEventArgs)
        If m_SynchronizingObject IsNot Nothing AndAlso m_SynchronizingObject.InvokeRequired Then
            Dim Parameters() As Object = {Me, e}
            m_SynchronizingObject.BeginInvoke(errorsd, Parameters)
            If Requests(e.TransactionNumber And 255) IsNot Nothing AndAlso Requests(e.TransactionNumber And 255).Count > 0 Then
                Requests(e.TransactionNumber And 255)(0).Response = e
                waitHandle(e.TransactionNumber And 255).Set()
            End If
        Else
            RaiseEvent ComError(Me, e)
        End If
    End Sub

    Private errorsd As New EventHandler(Of MfgControl.AdvancedHMI.Drivers.Common.PlcComEventArgs)(AddressOf ErrorReceivedSync)
    Private Sub ErrorReceivedSync(ByVal sender As Object, ByVal e As MfgControl.AdvancedHMI.Drivers.Common.PlcComEventArgs)
        RaiseEvent ComError(sender, e)
    End Sub
    '***********************************************************************************************************

    Private Sub UnsolictedMessageRcvdSync(ByVal sender As Object, ByVal e As EventArgs)
        RaiseEvent UnsolictedMessageRcvd(sender, e)
    End Sub
#End Region

End Class
