Public Class MainForm
    '*******************************************************************************
    '* Stop polling when the form is not visible in order to reduce communications
    '* Copy this section of code to every new form created
    '*******************************************************************************
    Private NotFirstShow As Boolean
    Private Sub Form_VisibleChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.VisibleChanged
        '* Do not start comms on first show in case it was set to disable in design mode
        If NotFirstShow Then
            AdvancedHMIDrivers.Utilities.StopComsOnHidden(components, Me)
        Else
            NotFirstShow = True
        End If
    End Sub



    Private Sub DataSubscriber1_DataChanged(sender As Object, e As Drivers.Common.PlcComEventArgs) Handles DataSubscriber1.DataChanged
        Dim i As Integer = DataSubscriber1.Value
        Dim Status As String
        Status = Convert.ToString(i, 2).PadLeft(8, "0") '8 bits
        'There are 8 bits that we need to check and account for on our screen. 
        'Modbus Decimal - 44139
        'Bit 0 - ALM3 - Alarm 3
        'Bit 1 - ALM2 - Alarm 2
        'Bit 2 - C degrees
        'Bit 3 - F degrees
        'Bit 4 - ALM1 - Alarm 1
        'Bit 5 - OUT 2 
        'Bit 6 - OUT 1
        'Bit 7 - AT - Auto Tuning

        'Alarm 3
        If Status.Substring(7, 1) = "0" Then
            Label9.BackColor = Color.Black
            Label9.ForeColor = Color.DarkGray
        Else
            Label9.BackColor = Color.LightGreen
            Label9.ForeColor = Color.Black
        End If
        'Alarm 2
        If Status.Substring(6, 1) = "0" Then
            Label8.BackColor = Color.Black
            Label8.ForeColor = Color.DarkGray
        Else
            Label8.BackColor = Color.LightGreen
            Label8.ForeColor = Color.Black
        End If
        'C degrees for Temperature
        If Status.Substring(5, 1) = "0" Then
            Label5.BackColor = Color.Black
            Label5.ForeColor = Color.DarkGray
        Else
            Label5.BackColor = Color.LightGreen
            Label5.ForeColor = Color.Black
        End If
        'F degrees for Temperature
        If Status.Substring(4, 1) = "0" Then
            Label7.BackColor = Color.Black
            Label7.ForeColor = Color.DarkGray
        Else
            Label7.BackColor = Color.LightGreen
            Label7.ForeColor = Color.Black
        End If
        'Alarm 1
        If Status.Substring(3, 1) = "0" Then
            Label10.BackColor = Color.Black
            Label10.ForeColor = Color.DarkGray
        Else
            Label10.BackColor = Color.LightGreen
            Label10.ForeColor = Color.Black
        End If
        'OUT2 - Output 2
        If Status.Substring(2, 1) = "0" Then
            Label4.BackColor = Color.Black
            Label4.ForeColor = Color.DarkGray
        Else
            Label4.BackColor = Color.LightGreen
            Label4.ForeColor = Color.Black
        End If
        'OUT1 - Output 1
        If Status.Substring(1, 1) = "0" Then
            Label3.BackColor = Color.Black
            Label3.ForeColor = Color.DarkGray
        Else
            Label3.BackColor = Color.LightGreen
            Label3.ForeColor = Color.Black
        End If
        'AT - Auto Tune
        If Status.Substring(0, 1) = "0" Then
            Label11.BackColor = Color.Black
            Label11.ForeColor = Color.DarkGray
        Else
            Label11.BackColor = Color.LightGreen
            Label11.ForeColor = Color.Black
        End If

        'Me.Refresh()
    End Sub
End Class
