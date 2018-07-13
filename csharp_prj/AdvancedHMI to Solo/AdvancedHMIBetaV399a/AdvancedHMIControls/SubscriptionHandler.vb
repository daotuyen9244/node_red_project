﻿Option Strict On
Public Class SubscriptionHandler
    Implements IDisposable


    Public Event DisplayError As EventHandler(Of MfgControl.AdvancedHMI.Drivers.Common.PlcComEventArgs)

#Region "Properties"
    '*****************************************************
    '* Property - Component to communicate to PLC through
    '*****************************************************
    Private m_CommComponent As AdvancedHMIDrivers.IComComponent
    Public Property CommComponent() As AdvancedHMIDrivers.IComComponent
        Get
            Return m_CommComponent
        End Get
        Set(ByVal value As AdvancedHMIDrivers.IComComponent)
            m_CommComponent = value
        End Set
    End Property

    Private m_Parent As Object
    Public Property Parent As Object
        Get
            Return m_Parent
        End Get
        Set(value As Object)
            m_Parent = value
        End Set
    End Property

    Private m_SubscriptionList As List(Of SubscriptionDetail)
    Public ReadOnly Property SubScriptionList As List(Of SubscriptionDetail)
        Get
            Return m_SubscriptionList
        End Get
    End Property
#End Region


#Region "Constructor/Destructor"
    Public Sub New()
        m_SubscriptionList = New List(Of SubscriptionDetail)
    End Sub

    Public Sub dispose() Implements System.IDisposable.Dispose
        dispose(True)
    End Sub

    '****************************************************************
    '* Control overrides dispose to clean up the component list.
    '****************************************************************
    Protected Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing Then
                If m_CommComponent IsNot Nothing Then
                    '* Unsubscribe from all
                    For i As Integer = 0 To m_SubscriptionList.Count - 1
                        m_CommComponent.Unsubscribe(m_SubscriptionList(i).NotificationID)
                    Next
                    m_SubscriptionList.Clear()
                End If
            End If
        Catch ex As Exception
        End Try
    End Sub
#End Region


    '******************************************************
    '* Attempt to create a subscription to the PLC driver
    '******************************************************
    Public Sub SubscribeTo(ByVal PLCAddress As String, ByVal callBack As EventHandler(Of SubscriptionHandlerEventArgs))
        SubscribeTo(PLCAddress, callBack, "")
    End Sub

    Public Sub SubscribeTo(ByVal PLCAddress As String, ByVal callBack As EventHandler(Of SubscriptionHandlerEventArgs), ByVal propertyName As String)
        '* Check to see if the subscription has already been created
        Dim index As Integer
        While index < m_SubscriptionList.Count AndAlso (m_SubscriptionList(index).CallBack <> callBack Or m_SubscriptionList(index).PropertyNameToSet <> propertyName)
            index += 1
        End While

        '* Already subscribed and PLCAddress was changed, so unsubscribe
        If (index < m_SubscriptionList.Count) AndAlso m_SubscriptionList(index).PLCAddress <> PLCAddress Then
            m_CommComponent.Unsubscribe(m_SubscriptionList(index).NotificationID)
            m_SubscriptionList.RemoveAt(index)
        End If

        '* Is there an address to subscribe to?
        If (PLCAddress IsNot Nothing) AndAlso (String.Compare(PLCAddress, "") <> 0) Then
            Try
                If m_CommComponent IsNot Nothing Then
                    '* If subscription succeedded, save the subscription details
                    Dim temp As New SubscriptionDetail(PLCAddress, callBack)
                    temp.PropertyNameToSet = propertyName
                    If PLCAddress.ToUpper.IndexOf("NOT ") = 0 Then
                        temp.Invert = True
                    End If
                    m_SubscriptionList.Add(temp)
                    InitializeTryTimer(500)
                Else
                    OnDisplayError("CommComponent Property not set")
                End If
            Catch ex As MfgControl.AdvancedHMI.Drivers.Common.PLCDriverException
                '* If subscribe fails, set up for retry
                InitializeSubscribeTry(ex, PLCAddress)
            End Try
        End If
    End Sub

    Public Sub UnsubscribeAll()
        If m_CommComponent IsNot Nothing Then
            For Each Subscript In m_SubscriptionList
                m_CommComponent.Unsubscribe(Subscript.NotificationID)
            Next
        End If
    End Sub

    Private Sub SubscribeToComDriver()
        If Not m_CommComponent.DisableSubscriptions Then
            For Each Subscript In m_SubscriptionList
                If Not Subscript.SuccessfullySubscribed Then
                    Dim address As String = Subscript.PLCAddress
                    If Subscript.Invert Then
                        address = Subscript.PLCAddress.Substring(4)
                    End If

                    If Not String.IsNullOrWhiteSpace(address.Trim) Then
                        Try
                            Dim NotificationID As Integer = m_CommComponent.Subscribe(address, 1, 250, AddressOf SubscribedDataReturned)
                            Subscript.NotificationID = NotificationID
                            Subscript.SuccessfullySubscribed = True
                        Catch ex As Exception
                            OnDisplayError(ex.Message)
                            Dim e As New MfgControl.AdvancedHMI.Drivers.Common.PLCDriverException(ex.Message)
                            'e.Message = ex.Message
                            InitializeSubscribeTry(e, Subscript.PLCAddress)
                        End Try
                    Else
                        '* Empty or null address
                        Dim dbg = 0
                    End If
                End If
            Next
        Else
            InitializeTryTimer(500)
        End If
    End Sub

    Public Sub SubscribeAutoProperties()
        '* Check through the properties looking for PLCAddress***, then see if the suffix matches an existing property
        Dim p() As Reflection.PropertyInfo = Parent.GetType().GetProperties


        For i = 0 To (p.Length - 1)
            If (p(i) IsNot Nothing) AndAlso (Not String.IsNullOrEmpty(p(i).Name)) AndAlso ((p(i).PropertyType) Is GetType(String)) Then ' (String.Compare(p(i).Name, "Container") <> 0) Then
                'Console.WriteLine(String.Concat("SAP1-", p(i).Name))
                '* Does this property start with "PLCAddress"?
                If p(i).Name.IndexOf("PLCAddress", StringComparison.CurrentCultureIgnoreCase) = 0 Then
                    'Console.WriteLine("SAP2")
                    '* Get the property value
                    Dim PLCAddress As String = DirectCast(p(i).GetValue(m_Parent, Nothing), String)
                    If Not (String.IsNullOrEmpty(PLCAddress)) Then
                        '* Get the text in the name after PLCAddress
                        Dim PropertyToWrite As String = p(i).Name.Substring(10)
                        Dim j As Integer = 0
                        '* See if there is a corresponding property with the extracted name
                        While j < p.Length AndAlso (String.Compare(p(j).Name, PropertyToWrite, True) <> 0)
                            j += 1
                        End While

                        '* If the proprty was found, then subscribe to the PLC Address
                        If j < p.Length Then
                            SubscribeTo(PLCAddress, Nothing, PropertyToWrite)
                        End If
                    End If
                End If
            End If
        Next

        'Console.WriteLine("Done with SubscribeAutoProperties")
    End Sub

    Private Sub SubscribedDataReturned(ByVal sender As Object, ByVal e As MfgControl.AdvancedHMI.Drivers.Common.PlcComEventArgs)
        For Each Subscript In m_SubscriptionList
            Dim address As String = Subscript.PLCAddress
            If Subscript.Invert Then
                address = Subscript.PLCAddress.Substring(4)
            End If

            If (e.PlcAddress Is Nothing) OrElse (String.Compare(address, e.PlcAddress, True) = 0) Then
                If e.ErrorId = 0 Then
                    Dim a As New SubscriptionHandlerEventArgs
                    a.PLCComEventArgs = e
                    a.SubscriptionDetail = Subscript

                    If e.Values IsNot Nothing AndAlso e.Values.Count > 0 Then
                        '* Check if the value should be inverted
                        If Subscript.Invert Then
                            '* Try to invert a boolean value
                            Try
                                Dim ea As MfgControl.AdvancedHMI.Drivers.Common.PlcComEventArgs
                                '* Clone the EventArgs for the inversion because there may be another subscription that doesn't need inverted
                                ea = CType(e.Clone, MfgControl.AdvancedHMI.Drivers.Common.PlcComEventArgs)
                                Dim x As New System.Collections.ObjectModel.Collection(Of String)

                                Dim s As String = (Convert.ToString(Not CBool(e.Values(0))))
                                x.Add(s)
                                ea.Values = x
                                a.PLCComEventArgs = ea
                            Catch ex As Exception
                                Dim dbg = 0
                            End Try
                        End If
                    Else
                        '* No data returned from driver
                        e.ErrorId = -999
                        e.ErrorMessage = "No Values Returned from Driver"
                    End If

                    If Subscript.CallBack IsNot Nothing Then
                        Subscript.CallBack.Invoke(sender, a)
                    End If

                    '********************************************
                    '* Process the AutoProperty subscribed items
                    '********************************************
                    If Me.Parent IsNot Nothing And a.SubscriptionDetail.PropertyNameToSet IsNot Nothing AndAlso (String.Compare(a.SubscriptionDetail.PropertyNameToSet, "") <> 0) Then
                        If e.ErrorId = 0 Then
                            Try
                                '* Does this property exist?
                                If m_Parent.GetType().GetProperty(a.SubscriptionDetail.PropertyNameToSet) IsNot Nothing Then

                                    '* Write the value to the property that came from the end of the PLCAddress... property name
                                    m_Parent.GetType().GetProperty(a.SubscriptionDetail.PropertyNameToSet). _
                                                SetValue(m_Parent, Utilities.DynamicConverter(a.PLCComEventArgs.Values(0), m_Parent.GetType().GetProperty(a.SubscriptionDetail.PropertyNameToSet).PropertyType), Nothing)
                                End If
                            Catch ex As Exception
                                OnDisplayError("INVALID VALUE RETURNED!" & a.PLCComEventArgs.Values(0))
                            End Try
                        Else
                            OnDisplayError("Com Error " & a.PLCComEventArgs.ErrorId & "." & a.PLCComEventArgs.ErrorMessage)
                        End If
                    End If
                Else
                    '* Error returned from driver
                    Try
                        Dim x As New System.Collections.ObjectModel.Collection(Of String)
                        x.Add(e.ErrorMessage)
                        e.Values = x

                        Dim a As New SubscriptionHandlerEventArgs
                        a.PLCComEventArgs = e
                        a.SubscriptionDetail = Subscript

                        'Subscript.CallBack.Invoke(sender, a)
                        OnDisplayError(e.ErrorMessage)
                    Catch ex As Exception
                        Dim dbg = 0
                    End Try
                End If
            End If
        Next
    End Sub

    '********************************************
    '* Show the error and start the retry time
    '********************************************
    Private Sub InitializeSubscribeTry(ByVal ex As MfgControl.AdvancedHMI.Drivers.Common.PLCDriverException, ByVal PLCAddress As String)
        '* Error 1808 is from TwinCAT ADS
        'If ex.ErrorCode = 1808 Then
        'OnDisplayError("""" & PLCAddress & """ PLC Address not found")
        'Else
        OnDisplayError(ex.Message)
        'End If

        InitializeTryTimer(10000)
    End Sub

    Private Sub InitializeTryTimer(ByVal interval As Integer)
        If SubscribeTryTimer Is Nothing Then
            SubscribeTryTimer = New Windows.Forms.Timer
            SubscribeTryTimer.Interval = interval
            AddHandler SubscribeTryTimer.Tick, AddressOf SubscribeTry_Tick
        End If

        SubscribeTryTimer.Enabled = True
    End Sub



    '********************************************
    '* Keep retrying to subscribe if it failed
    '********************************************
    Private SubscribeTryTimer As Windows.Forms.Timer
    Private Sub SubscribeTry_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs)
        SubscribeTryTimer.Enabled = False
        SubscribeTryTimer.Dispose()
        SubscribeTryTimer = Nothing

        SubscribeToComDriver()
    End Sub


    Protected Overridable Sub OnDisplayError(ByVal msg As String)
        Dim e As New MfgControl.AdvancedHMI.Drivers.Common.PlcComEventArgs(0, msg)

        RaiseEvent DisplayError(Me, e)
    End Sub

End Class
