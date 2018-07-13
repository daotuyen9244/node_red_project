<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()>
Partial Class MainForm
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    '   <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    ' <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container()
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(MainForm))
        Me.Label1 = New System.Windows.Forms.Label()
        Me.QuickStartLabel = New System.Windows.Forms.Label()
        Me.PictureBox1 = New System.Windows.Forms.PictureBox()
        Me.PictureBox2 = New System.Windows.Forms.PictureBox()
        Me.ShapeContainer1 = New Microsoft.VisualBasic.PowerPacks.ShapeContainer()
        Me.RectangleShape1 = New Microsoft.VisualBasic.PowerPacks.RectangleShape()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.Label5 = New System.Windows.Forms.Label()
        Me.Label6 = New System.Windows.Forms.Label()
        Me.Label7 = New System.Windows.Forms.Label()
        Me.Label8 = New System.Windows.Forms.Label()
        Me.Label9 = New System.Windows.Forms.Label()
        Me.Label10 = New System.Windows.Forms.Label()
        Me.Label11 = New System.Windows.Forms.Label()
        Me.Label12 = New System.Windows.Forms.Label()
        Me.Label13 = New System.Windows.Forms.Label()
        Me.DigitalPanelMeter2 = New AdvancedHMIControls.DigitalPanelMeter()
        Me.ModbusRTUCom1 = New AdvancedHMIDrivers.ModbusRTU.ModbusRTUCom(Me.components)
        Me.BasicTrendChart1 = New AdvancedHMIControls.BasicTrendChart()
        Me.DigitalPanelMeter1 = New AdvancedHMIControls.DigitalPanelMeter()
        Me.DataSubscriber1 = New AdvancedHMIControls.DataSubscriber(Me.components)
        CType(Me.PictureBox1, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.PictureBox2, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.DataSubscriber1, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'Label1
        '
        Me.Label1.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left), System.Windows.Forms.AnchorStyles)
        Me.Label1.BackColor = System.Drawing.Color.Transparent
        Me.Label1.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.0!)
        Me.Label1.ForeColor = System.Drawing.Color.FromArgb(CType(CType(64, Byte), Integer), CType(CType(64, Byte), Integer), CType(CType(64, Byte), Integer))
        Me.Label1.Location = New System.Drawing.Point(12, 395)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(194, 32)
        Me.Label1.TabIndex = 3
        Me.Label1.Text = "For Development Source Code Visit" & Global.Microsoft.VisualBasic.ChrW(13) & Global.Microsoft.VisualBasic.ChrW(10) & "http://www.advancedhmi.com"
        Me.Label1.TextAlign = System.Drawing.ContentAlignment.TopCenter
        '
        'QuickStartLabel
        '
        Me.QuickStartLabel.AutoSize = True
        Me.QuickStartLabel.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.0!, System.Drawing.FontStyle.Bold)
        Me.QuickStartLabel.ForeColor = System.Drawing.Color.White
        Me.QuickStartLabel.Location = New System.Drawing.Point(12, 9)
        Me.QuickStartLabel.Name = "QuickStartLabel"
        Me.QuickStartLabel.Size = New System.Drawing.Size(273, 117)
        Me.QuickStartLabel.TabIndex = 38
        Me.QuickStartLabel.Text = resources.GetString("QuickStartLabel.Text")
        Me.QuickStartLabel.Visible = False
        '
        'PictureBox1
        '
        Me.PictureBox1.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.PictureBox1.BackgroundImage = Global.MfgControl.AdvancedHMI.My.Resources.Resources.AdvancedHMILogoBR
        Me.PictureBox1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom
        Me.PictureBox1.Location = New System.Drawing.Point(447, 12)
        Me.PictureBox1.Name = "PictureBox1"
        Me.PictureBox1.Size = New System.Drawing.Size(322, 47)
        Me.PictureBox1.TabIndex = 42
        Me.PictureBox1.TabStop = False
        '
        'PictureBox2
        '
        Me.PictureBox2.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.PictureBox2.BackgroundImage = CType(resources.GetObject("PictureBox2.BackgroundImage"), System.Drawing.Image)
        Me.PictureBox2.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom
        Me.PictureBox2.Location = New System.Drawing.Point(365, 12)
        Me.PictureBox2.Name = "PictureBox2"
        Me.PictureBox2.Size = New System.Drawing.Size(52, 47)
        Me.PictureBox2.TabIndex = 45
        Me.PictureBox2.TabStop = False
        '
        'ShapeContainer1
        '
        Me.ShapeContainer1.Location = New System.Drawing.Point(0, 0)
        Me.ShapeContainer1.Margin = New System.Windows.Forms.Padding(0)
        Me.ShapeContainer1.Name = "ShapeContainer1"
        Me.ShapeContainer1.Shapes.AddRange(New Microsoft.VisualBasic.PowerPacks.Shape() {Me.RectangleShape1})
        Me.ShapeContainer1.Size = New System.Drawing.Size(781, 445)
        Me.ShapeContainer1.TabIndex = 50
        Me.ShapeContainer1.TabStop = False
        '
        'RectangleShape1
        '
        Me.RectangleShape1.BorderColor = System.Drawing.Color.LightGreen
        Me.RectangleShape1.BorderWidth = 5
        Me.RectangleShape1.Location = New System.Drawing.Point(523, 73)
        Me.RectangleShape1.Name = "RectangleShape1"
        Me.RectangleShape1.Size = New System.Drawing.Size(231, 356)
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.ForeColor = System.Drawing.Color.DarkGray
        Me.Label2.Location = New System.Drawing.Point(532, 82)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(214, 13)
        Me.Label2.TabIndex = 51
        Me.Label2.Text = "Solo Process Temperature Controller"
        '
        'Label3
        '
        Me.Label3.AutoSize = True
        Me.Label3.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label3.ForeColor = System.Drawing.Color.DarkGray
        Me.Label3.Location = New System.Drawing.Point(536, 346)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(40, 13)
        Me.Label3.TabIndex = 72
        Me.Label3.Text = "OUT1"
        '
        'Label4
        '
        Me.Label4.AutoSize = True
        Me.Label4.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label4.ForeColor = System.Drawing.Color.DarkGray
        Me.Label4.Location = New System.Drawing.Point(598, 346)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(40, 13)
        Me.Label4.TabIndex = 73
        Me.Label4.Text = "OUT2"
        '
        'Label5
        '
        Me.Label5.AutoSize = True
        Me.Label5.BackColor = System.Drawing.Color.Black
        Me.Label5.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label5.ForeColor = System.Drawing.Color.DarkGray
        Me.Label5.Location = New System.Drawing.Point(711, 346)
        Me.Label5.Name = "Label5"
        Me.Label5.Size = New System.Drawing.Size(24, 13)
        Me.Label5.TabIndex = 75
        Me.Label5.Text = "° C"
        '
        'Label6
        '
        Me.Label6.AutoSize = True
        Me.Label6.Font = New System.Drawing.Font("Microsoft Sans Serif", 6.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label6.ForeColor = System.Drawing.Color.DarkGray
        Me.Label6.Location = New System.Drawing.Point(653, 333)
        Me.Label6.Name = "Label6"
        Me.Label6.Size = New System.Drawing.Size(0, 9)
        Me.Label6.TabIndex = 74
        '
        'Label7
        '
        Me.Label7.AutoSize = True
        Me.Label7.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label7.ForeColor = System.Drawing.Color.DarkGray
        Me.Label7.Location = New System.Drawing.Point(658, 346)
        Me.Label7.Name = "Label7"
        Me.Label7.Size = New System.Drawing.Size(23, 13)
        Me.Label7.TabIndex = 76
        Me.Label7.Text = "° F"
        '
        'Label8
        '
        Me.Label8.AutoSize = True
        Me.Label8.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label8.ForeColor = System.Drawing.Color.DarkGray
        Me.Label8.Location = New System.Drawing.Point(652, 395)
        Me.Label8.Name = "Label8"
        Me.Label8.Size = New System.Drawing.Size(39, 13)
        Me.Label8.TabIndex = 80
        Me.Label8.Text = "ALM2"
        '
        'Label9
        '
        Me.Label9.AutoSize = True
        Me.Label9.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label9.ForeColor = System.Drawing.Color.DarkGray
        Me.Label9.Location = New System.Drawing.Point(705, 395)
        Me.Label9.Name = "Label9"
        Me.Label9.Size = New System.Drawing.Size(39, 13)
        Me.Label9.TabIndex = 79
        Me.Label9.Text = "ALM3"
        '
        'Label10
        '
        Me.Label10.AutoSize = True
        Me.Label10.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label10.ForeColor = System.Drawing.Color.DarkGray
        Me.Label10.Location = New System.Drawing.Point(593, 395)
        Me.Label10.Name = "Label10"
        Me.Label10.Size = New System.Drawing.Size(39, 13)
        Me.Label10.TabIndex = 78
        Me.Label10.Text = "ALM1"
        '
        'Label11
        '
        Me.Label11.AutoSize = True
        Me.Label11.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label11.ForeColor = System.Drawing.Color.DarkGray
        Me.Label11.Location = New System.Drawing.Point(536, 395)
        Me.Label11.Name = "Label11"
        Me.Label11.Size = New System.Drawing.Size(23, 13)
        Me.Label11.TabIndex = 77
        Me.Label11.Text = "AT"
        '
        'Label12
        '
        Me.Label12.AutoSize = True
        Me.Label12.ForeColor = System.Drawing.Color.DarkGray
        Me.Label12.Location = New System.Drawing.Point(25, 135)
        Me.Label12.Name = "Label12"
        Me.Label12.Size = New System.Drawing.Size(285, 13)
        Me.Label12.TabIndex = 88
        Me.Label12.Text = "Solo Process Temperature Controller Trend Chart"
        '
        'Label13
        '
        Me.Label13.AutoSize = True
        Me.Label13.ForeColor = System.Drawing.Color.DarkGray
        Me.Label13.Location = New System.Drawing.Point(112, 362)
        Me.Label13.Name = "Label13"
        Me.Label13.Size = New System.Drawing.Size(232, 13)
        Me.Label13.TabIndex = 89
        Me.Label13.Text = "5 Minutes - Polling 250ms x 1200 points"
        '
        'DigitalPanelMeter2
        '
        Me.DigitalPanelMeter2.BackColor = System.Drawing.Color.LightGreen
        Me.DigitalPanelMeter2.CommComponent = Me.ModbusRTUCom1
        Me.DigitalPanelMeter2.DecimalPosition = 1
        Me.DigitalPanelMeter2.ForeColor = System.Drawing.Color.LightGreen
        Me.DigitalPanelMeter2.KeypadFontColor = System.Drawing.Color.WhiteSmoke
        Me.DigitalPanelMeter2.KeypadMaxValue = 0R
        Me.DigitalPanelMeter2.KeypadMinValue = 0R
        Me.DigitalPanelMeter2.KeypadScaleFactor = 0.1R
        Me.DigitalPanelMeter2.KeypadText = Nothing
        Me.DigitalPanelMeter2.KeypadWidth = 300
        Me.DigitalPanelMeter2.Location = New System.Drawing.Point(526, 214)
        Me.DigitalPanelMeter2.Name = "DigitalPanelMeter2"
        Me.DigitalPanelMeter2.NumberOfDigits = 4
        Me.DigitalPanelMeter2.PLCAddressKeypad = "44098"
        Me.DigitalPanelMeter2.PLCAddressText = ""
        Me.DigitalPanelMeter2.PLCAddressValue = "44098"
        Me.DigitalPanelMeter2.PLCAddressVisible = ""
        Me.DigitalPanelMeter2.Resolution = New Decimal(New Integer() {1, 0, 0, 0})
        Me.DigitalPanelMeter2.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.DigitalPanelMeter2.Size = New System.Drawing.Size(226, 116)
        Me.DigitalPanelMeter2.TabIndex = 49
        Me.DigitalPanelMeter2.Text = "SV"
        Me.DigitalPanelMeter2.Value = 0R
        Me.DigitalPanelMeter2.ValueScaleFactor = New Decimal(New Integer() {1, 0, 0, 0})
        Me.DigitalPanelMeter2.ValueScaleOffset = New Decimal(New Integer() {0, 0, 0, 0})
        '
        'ModbusRTUCom1
        '
        Me.ModbusRTUCom1.BaudRate = 9600
        Me.ModbusRTUCom1.DataBits = 8
        Me.ModbusRTUCom1.DisableSubscriptions = False
        Me.ModbusRTUCom1.Parity = System.IO.Ports.Parity.Even
        Me.ModbusRTUCom1.PollRateOverride = 250
        Me.ModbusRTUCom1.PortName = "COM5"
        Me.ModbusRTUCom1.StationAddress = CType(1, Byte)
        Me.ModbusRTUCom1.StopBits = System.IO.Ports.StopBits.One
        Me.ModbusRTUCom1.SwapBytes = True
        Me.ModbusRTUCom1.SwapWords = False
        Me.ModbusRTUCom1.SynchronizingObject = Me
        '
        'BasicTrendChart1
        '
        Me.BasicTrendChart1.BackColor = System.Drawing.Color.Black
        Me.BasicTrendChart1.CommComponent = Me.ModbusRTUCom1
        Me.BasicTrendChart1.ForeColor = System.Drawing.Color.White
        Me.BasicTrendChart1.Location = New System.Drawing.Point(28, 166)
        Me.BasicTrendChart1.MaxPoints = 1200
        Me.BasicTrendChart1.Name = "BasicTrendChart1"
        Me.BasicTrendChart1.PLCAddressValue = "44097"
        Me.BasicTrendChart1.PLCAddressVisible = "44097"
        Me.BasicTrendChart1.Size = New System.Drawing.Size(454, 193)
        Me.BasicTrendChart1.TabIndex = 48
        Me.BasicTrendChart1.Text = "BasicTrendChart1"
        Me.BasicTrendChart1.Value = ""
        Me.BasicTrendChart1.YMaximum = 350
        Me.BasicTrendChart1.YMinimum = 150
        '
        'DigitalPanelMeter1
        '
        Me.DigitalPanelMeter1.BackColor = System.Drawing.Color.Black
        Me.DigitalPanelMeter1.CommComponent = Me.ModbusRTUCom1
        Me.DigitalPanelMeter1.DecimalPosition = 1
        Me.DigitalPanelMeter1.ForeColor = System.Drawing.Color.IndianRed
        Me.DigitalPanelMeter1.KeypadFontColor = System.Drawing.Color.WhiteSmoke
        Me.DigitalPanelMeter1.KeypadMaxValue = 0R
        Me.DigitalPanelMeter1.KeypadMinValue = 0R
        Me.DigitalPanelMeter1.KeypadScaleFactor = 0.1R
        Me.DigitalPanelMeter1.KeypadText = Nothing
        Me.DigitalPanelMeter1.KeypadWidth = 300
        Me.DigitalPanelMeter1.Location = New System.Drawing.Point(526, 101)
        Me.DigitalPanelMeter1.Name = "DigitalPanelMeter1"
        Me.DigitalPanelMeter1.NumberOfDigits = 4
        Me.DigitalPanelMeter1.PLCAddressKeypad = ""
        Me.DigitalPanelMeter1.PLCAddressText = ""
        Me.DigitalPanelMeter1.PLCAddressValue = "44097"
        Me.DigitalPanelMeter1.PLCAddressVisible = ""
        Me.DigitalPanelMeter1.Resolution = New Decimal(New Integer() {1, 0, 0, 0})
        Me.DigitalPanelMeter1.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.DigitalPanelMeter1.Size = New System.Drawing.Size(226, 116)
        Me.DigitalPanelMeter1.TabIndex = 46
        Me.DigitalPanelMeter1.Text = "PV"
        Me.DigitalPanelMeter1.Value = 0R
        Me.DigitalPanelMeter1.ValueScaleFactor = New Decimal(New Integer() {1, 0, 0, 0})
        Me.DigitalPanelMeter1.ValueScaleOffset = New Decimal(New Integer() {0, 0, 0, 0})
        '
        'DataSubscriber1
        '
        Me.DataSubscriber1.CommComponent = Me.ModbusRTUCom1
        Me.DataSubscriber1.PLCAddressValue = "44139"
        Me.DataSubscriber1.PollRate = 0
        Me.DataSubscriber1.SynchronizingObject = Me
        Me.DataSubscriber1.Value = Nothing
        '
        'MainForm
        '
        Me.AutoScroll = True
        Me.AutoSize = True
        Me.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink
        Me.BackColor = System.Drawing.Color.Black
        Me.ClientSize = New System.Drawing.Size(781, 445)
        Me.Controls.Add(Me.Label13)
        Me.Controls.Add(Me.Label12)
        Me.Controls.Add(Me.Label8)
        Me.Controls.Add(Me.Label9)
        Me.Controls.Add(Me.Label10)
        Me.Controls.Add(Me.Label11)
        Me.Controls.Add(Me.Label7)
        Me.Controls.Add(Me.Label5)
        Me.Controls.Add(Me.Label6)
        Me.Controls.Add(Me.Label4)
        Me.Controls.Add(Me.Label3)
        Me.Controls.Add(Me.Label2)
        Me.Controls.Add(Me.DigitalPanelMeter2)
        Me.Controls.Add(Me.BasicTrendChart1)
        Me.Controls.Add(Me.DigitalPanelMeter1)
        Me.Controls.Add(Me.PictureBox2)
        Me.Controls.Add(Me.PictureBox1)
        Me.Controls.Add(Me.QuickStartLabel)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.ShapeContainer1)
        Me.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ForeColor = System.Drawing.Color.White
        Me.ImeMode = System.Windows.Forms.ImeMode.NoControl
        Me.Name = "MainForm"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.Manual
        Me.Text = "AdvancedHMI v3.99a"
        CType(Me.PictureBox1, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.PictureBox2, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.DataSubscriber1, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents QuickStartLabel As System.Windows.Forms.Label
    Friend WithEvents PictureBox1 As System.Windows.Forms.PictureBox
    Friend WithEvents PictureBox2 As PictureBox
    Friend WithEvents DigitalPanelMeter1 As AdvancedHMIControls.DigitalPanelMeter
    Friend WithEvents ModbusRTUCom1 As AdvancedHMIDrivers.ModbusRTU.ModbusRTUCom
    Friend WithEvents BasicTrendChart1 As AdvancedHMIControls.BasicTrendChart
    Friend WithEvents DigitalPanelMeter2 As AdvancedHMIControls.DigitalPanelMeter
    Friend WithEvents Label2 As Label
    Friend WithEvents ShapeContainer1 As PowerPacks.ShapeContainer
    Friend WithEvents RectangleShape1 As PowerPacks.RectangleShape
    Friend WithEvents Label3 As Label
    Friend WithEvents Label8 As Label
    Friend WithEvents Label9 As Label
    Friend WithEvents Label10 As Label
    Friend WithEvents Label11 As Label
    Friend WithEvents Label7 As Label
    Friend WithEvents Label5 As Label
    Friend WithEvents Label6 As Label
    Friend WithEvents Label4 As Label
    Friend WithEvents Label13 As Label
    Friend WithEvents Label12 As Label
    Friend WithEvents DataSubscriber1 As AdvancedHMIControls.DataSubscriber
End Class
