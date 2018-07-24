namespace MongoDBTest2
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.btnSave = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.txtName = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.txtAge = new System.Windows.Forms.TextBox();
            this.txtLocation = new System.Windows.Forms.TextBox();
            this.rbEntity1 = new System.Windows.Forms.RadioButton();
            this.rbEntity2 = new System.Windows.Forms.RadioButton();
            this.rbEntity3 = new System.Windows.Forms.RadioButton();
            this.btnFind = new System.Windows.Forms.Button();
            this.btnFindAll = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // btnSave
            // 
            this.btnSave.Location = new System.Drawing.Point(38, 228);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(75, 23);
            this.btnSave.TabIndex = 0;
            this.btnSave.Text = "Save";
            this.btnSave.UseVisualStyleBackColor = true;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(69, 101);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(44, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Name : ";
            // 
            // txtName
            // 
            this.txtName.Location = new System.Drawing.Point(147, 101);
            this.txtName.Name = "txtName";
            this.txtName.Size = new System.Drawing.Size(100, 20);
            this.txtName.TabIndex = 2;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(69, 137);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(26, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Age";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(69, 175);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(48, 13);
            this.label3.TabIndex = 4;
            this.label3.Text = "Location";
            // 
            // txtAge
            // 
            this.txtAge.Location = new System.Drawing.Point(147, 137);
            this.txtAge.MaxLength = 2;
            this.txtAge.Name = "txtAge";
            this.txtAge.Size = new System.Drawing.Size(100, 20);
            this.txtAge.TabIndex = 5;
            // 
            // txtLocation
            // 
            this.txtLocation.Location = new System.Drawing.Point(147, 167);
            this.txtLocation.Name = "txtLocation";
            this.txtLocation.Size = new System.Drawing.Size(100, 20);
            this.txtLocation.TabIndex = 6;
            // 
            // rbEntity1
            // 
            this.rbEntity1.AutoSize = true;
            this.rbEntity1.Checked = true;
            this.rbEntity1.Location = new System.Drawing.Point(57, 42);
            this.rbEntity1.Name = "rbEntity1";
            this.rbEntity1.Size = new System.Drawing.Size(57, 17);
            this.rbEntity1.TabIndex = 7;
            this.rbEntity1.TabStop = true;
            this.rbEntity1.Text = "Entity1";
            this.rbEntity1.UseVisualStyleBackColor = true;
            this.rbEntity1.CheckedChanged += new System.EventHandler(this.rbEntity1_CheckedChanged);
            // 
            // rbEntity2
            // 
            this.rbEntity2.AutoSize = true;
            this.rbEntity2.Location = new System.Drawing.Point(147, 42);
            this.rbEntity2.Name = "rbEntity2";
            this.rbEntity2.Size = new System.Drawing.Size(57, 17);
            this.rbEntity2.TabIndex = 7;
            this.rbEntity2.Text = "Entity2";
            this.rbEntity2.UseVisualStyleBackColor = true;
            this.rbEntity2.CheckedChanged += new System.EventHandler(this.rbEntity2_CheckedChanged);
            // 
            // rbEntity3
            // 
            this.rbEntity3.AutoSize = true;
            this.rbEntity3.Location = new System.Drawing.Point(245, 42);
            this.rbEntity3.Name = "rbEntity3";
            this.rbEntity3.Size = new System.Drawing.Size(57, 17);
            this.rbEntity3.TabIndex = 7;
            this.rbEntity3.Text = "Entity3";
            this.rbEntity3.UseVisualStyleBackColor = true;
            this.rbEntity3.CheckedChanged += new System.EventHandler(this.rbEntity3_CheckedChanged);
            // 
            // btnFind
            // 
            this.btnFind.Location = new System.Drawing.Point(147, 228);
            this.btnFind.Name = "btnFind";
            this.btnFind.Size = new System.Drawing.Size(100, 23);
            this.btnFind.TabIndex = 8;
            this.btnFind.Text = "Find By Name";
            this.btnFind.UseVisualStyleBackColor = true;
            this.btnFind.Click += new System.EventHandler(this.btnFind_Click);
            // 
            // btnFindAll
            // 
            this.btnFindAll.Location = new System.Drawing.Point(38, 268);
            this.btnFindAll.Name = "btnFindAll";
            this.btnFindAll.Size = new System.Drawing.Size(75, 23);
            this.btnFindAll.TabIndex = 9;
            this.btnFindAll.Text = "Find All";
            this.btnFindAll.UseVisualStyleBackColor = true;
            this.btnFindAll.Click += new System.EventHandler(this.btnFindAll_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(833, 329);
            this.Controls.Add(this.btnFindAll);
            this.Controls.Add(this.btnFind);
            this.Controls.Add(this.rbEntity3);
            this.Controls.Add(this.rbEntity2);
            this.Controls.Add(this.rbEntity1);
            this.Controls.Add(this.txtLocation);
            this.Controls.Add(this.txtAge);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.txtName);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btnSave);
            this.Name = "Form1";
            this.Text = "Mongo Database Test with C#";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtName;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox txtAge;
        private System.Windows.Forms.TextBox txtLocation;
        private System.Windows.Forms.RadioButton rbEntity1;
        private System.Windows.Forms.RadioButton rbEntity2;
        private System.Windows.Forms.RadioButton rbEntity3;
        private System.Windows.Forms.Button btnFind;
        private System.Windows.Forms.Button btnFindAll;
        
    }
}

