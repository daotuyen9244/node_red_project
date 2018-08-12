using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
namespace Test_export_data
{
    public partial class Form1 : Form
    {

        Random rnd = new Random();
        public struct DataOneStation
        {
            public UInt64 Id_station;
            public UInt64[] DataByDay;
            public UInt64[] DataByMon;
            public UInt64   DataByYear;
        }
        public struct DataOneStation1
        {
            public UInt64 Id_station;
            public UInt64[,] Data;
            public UInt64[] DataByMon;
            public UInt64 DataByYear;
        }
        //DataAllStation my_data ;
        DataOneStation[] my_data = new DataOneStation[5];
        DataOneStation1[] my_data1 = new DataOneStation1[5];
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            
            for (int cr = 0;cr < 5;cr++)
            {
                my_data[cr].DataByDay = new UInt64[31];
                my_data[cr].DataByMon = new UInt64[12];
            }
            for (int cr = 0; cr < 5; cr++)
            {
                my_data1[cr].Data = new UInt64[31,12];
                my_data1[cr].DataByMon = new UInt64[12];
            }
            string stringData = "";
            for(UInt64 st =0;st<5;st++)
            {
                my_data[st].Id_station = st+1;
                stringData += "Data station:  " + (st + 1).ToString() + "\n";
                for (UInt64 dm = 0; dm < 12; dm++)
                {
                    for (UInt64 dd = 0; dd < 31; dd++)
                    {
                        my_data[st].DataByDay[dd] = UInt64.Parse(rnd.Next(1, 100).ToString());
                        my_data[st].DataByMon[dm] += my_data[st].DataByDay[dd];

                        ////////////////
                        my_data1[st].Data[dd,dm] = UInt64.Parse(rnd.Next(1, 100).ToString());
                        my_data1[st].DataByMon[dm] += my_data1[st].Data[dd,dm];

                    }
                    my_data[st].DataByYear += my_data[st].DataByMon[dm];
                    my_data1[st].DataByYear += my_data1[st].DataByMon[dm];

                    stringData += "Data thang " + (my_data[st].DataByMon[dm]).ToString() + "\n";
                    stringData += "Data thang1 " + (my_data1[st].DataByMon[dm]).ToString() + "\n";
                    
                }
                stringData += "Data nam " + (my_data[st].DataByYear).ToString() + "\n";
                stringData += "Data nam1 " + (my_data1[st].DataByYear).ToString() + "\n";
                stringData += "------------------------------------\n";
            }
            richTextBox1.Text = "";
            //richTextBox1.Text = stringData;

            string path = @"c:\test\MyTest";
            string sub_path = ".csv";
            path += sub_path;
            if (File.Exists(path))
            {
                // Use a try block to catch IOExceptions, to
                // handle the case of the file already being
                // opened by another process.
                try
                {
                    File.Delete(path);
                }
                catch (Exception ex)
                {
                    
                }
            }

            if (!File.Exists(path))
            {
                
                // Create a file to write to.
                using (StreamWriter sw = File.CreateText(path))
                {
                    for (UInt64 dd = 0; dd < 32; dd++)
                    {
                        if (dd == 0)
                        {
                            sw.Write( ",");
                        }
                        else if (dd == 31)
                        {
                            sw.WriteLine("Day 31 ");
                        }
                        else
                        {
                            sw.Write("Day " + (dd ) + ",");
                        }
                    }
                    for (UInt64 dm = 0; dm < 12; dm++)
                    {
                        
                        for (UInt64 dd = 0; dd < 32; dd++)
                        {

                            if (dd == 0)
                            {
                                sw.Write("Thang " + (dm+1) +",");
                            }
                            else if (dd == 31)
                            {
                                sw.WriteLine(my_data1[0].Data[dd-1, dm]);
                            }
                            else
                            {
                                sw.Write(my_data1[0].Data[dd-1, dm] + ",");
                            }
                        }
                        
                    }
                }
            }
            using (StreamReader sr = File.OpenText(path))
            {
                string s = "";
                while ((s = sr.ReadLine()) != null)
                {
                    richTextBox1.Text += s;
                }
                
            }
        }
    }
}
