using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;
namespace run_cmd
{
    public partial class Form1 : Form
    {
       
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {

            ProcessStartInfo pro = new ProcessStartInfo();
            pro.FileName = "cmd.exe";
            pro.WorkingDirectory = @"D:\PowerDAQ\MongoDB\Server\4.0\bin";
            pro.Arguments = "/C mongod.exe --dbpath D:\\PowerDAQ\\MongoDB\\database";
            pro.WindowStyle = System.Diagnostics.ProcessWindowStyle.Minimized;
            Process proStart = new Process();
            proStart.StartInfo = pro;
            proStart.Start();
            
        }

        private void button2_Click(object sender, EventArgs e)
        {
            ProcessStartInfo pro = new ProcessStartInfo();
            pro.FileName = "cmd.exe";
            pro.WorkingDirectory = @"D:\PowerDAQ\MongoDB\Server\4.0\bin";
            pro.Arguments = "/C mongoexport -d PowerDAQ -c PowerCollection -f Id_station,Data,OldData,Date,Month,Year --csv -o D:/PowerDAQ/ExportData/department.csv";
            pro.WindowStyle = System.Diagnostics.ProcessWindowStyle.Minimized;
            Process proStart = new Process();
            proStart.StartInfo = pro;
            proStart.Start();
            proStart.Close();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            
            ProcessStartInfo pro = new ProcessStartInfo();
            pro.FileName = "cmd.exe";
            pro.WorkingDirectory = @"D:\PowerDAQ\MongoDB\Server\4.0\bin";

            pro.Arguments = "/C mongoimport -d PowerDAQ -c PowerCollections --type csv --file  D:/PowerDAQ/ExportData/department.csv --headerline ";
            pro.WindowStyle = System.Diagnostics.ProcessWindowStyle.Minimized;
            Process proStart = new Process();
            proStart.StartInfo = pro;
            proStart.Start();
            proStart.Close();
        }
    }
}
