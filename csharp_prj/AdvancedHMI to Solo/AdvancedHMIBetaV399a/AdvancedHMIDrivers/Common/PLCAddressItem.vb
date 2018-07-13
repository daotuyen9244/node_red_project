<System.ComponentModel.TypeConverter(GetType(PLCAddressItemTypeConverter))> _
Public Class PLCAddressItem

#Region "Properties"
    Private m_PLCAddress As String
    Public Property PLCAddress As String
        Get
            Return m_PLCAddress
        End Get
        Set(value As String)
            m_PLCAddress = value
        End Set
    End Property

    Private m_NumberOfElements As Integer
    Public Property NumberOfElements As Integer
        Get
            Return m_NumberOfElements
        End Get
        Set(value As Integer)
            m_NumberOfElements = value
        End Set
    End Property

    Private m_ScaleFactor As Double
    Public Property ScaleFactor As Double
        Get
            Return m_ScaleFactor
        End Get
        Set(value As Double)
            m_ScaleFactor = value
        End Set
    End Property

    Private m_ScaleOffset As Double
    Public Property ScaleOffset As Double
        Get
            Return m_ScaleOffset
        End Get
        Set(value As Double)
            m_ScaleOffset = value
        End Set
    End Property

    Private m_LastValue As String
    <System.ComponentModel.Browsable(False)> _
    Public Property LastValue As String
        Get
            Return m_LastValue
        End Get
        Set(value As String)
            m_LastValue = value
        End Set
    End Property

    Private m_SubscriptionID As Integer
    <System.ComponentModel.Browsable(False)> _
    Public Property SubscriptionID As Integer
        Get
            Return m_SubscriptionID
        End Get
        Set(value As Integer)
            m_SubscriptionID = value
        End Set
    End Property

    'Public RawValue As String
#End Region

#Region "Constructor"
    Public Sub New()
        m_NumberOfElements = 1
        m_ScaleFactor = 1
    End Sub

    Public Sub New(plcAddress As String)
        Me.New()

        m_PLCAddress = plcAddress
    End Sub

    Public Sub New(plcAddress As String, numberOfElements As Integer)
        Me.New(plcAddress)

        m_NumberOfElements = numberOfElements
    End Sub
#End Region

    Public Function GetScaledValue(ByVal v As String) As String
        If m_ScaleFactor = 1 And m_ScaleOffset = 0 Then
            Return v
        Else
            Dim Result As Double
            If Double.TryParse(v, Result) Then
                Return CStr(Result * m_ScaleFactor + m_ScaleOffset)
            Else
                Return v
            End If
        End If
    End Function
End Class
