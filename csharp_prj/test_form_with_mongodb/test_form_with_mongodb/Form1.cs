using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using MongoDB.Bson;
using MongoDB.Driver;
using MongoDB.Driver.Builders;
using MongoDB.Driver.GridFS;
using MongoDB.Driver.Linq;
using System.Data.SqlClient;

namespace test_form_with_mongodb
{
    public partial class Form1 : Form
    {
        MongoServer _server;
        MongoDatabase _database;
        MongoCollection _collection;
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            string connection = "mongodb://localhost:27017";
            //string connection = ConfigurationSettings.AppSettings["Connection"];

            _server = MongoServer.Create(connection);
            _database = _server.GetDatabase("PowerDAQ", SafeMode.True);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            var _users = _database.GetCollection<Station>("PowerCollection");
            var user = new Station { };
            user.Id_station = "1";
            user.Data = "5665";
            user.Date = "20";
            user.Month = "7";
            user.Year = "2018";
            //user.Age = Convert.ToInt32(txtAge.Text);
            //user.Name = txtName.Text;
            //user.Location = txtLocation.Text;
            _users.Insert(user);
            var id = user.Id_station;
        }
        

        
        public class Station
        {
            public ObjectId Id { get; set; }

            public string Id_station { get; set; }

            public string Data { get; set; }

            public string OldData { get; set; }

            public string Date { get; set; }

            public string Month { get; set; }

            public string Year { get; set; }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            _collection = _database.GetCollection<Station>("PowerCollection");
            IMongoQuery query = Query.EQ("Id_station", "1");
            Station _user = _collection.FindAs<Station>(query).FirstOrDefault();
            if (_user != null)
            {
                MessageBox.Show(_user.Data.ToString());
            }
            else
            {
                MessageBox.Show("Not able to find any results with name " );
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
             
            _collection = _database.GetCollection<Station>("PowerCollection");
            IMongoQuery query = Query.EQ("Id_station", "1");
            Station _user = _collection.FindAs<Station>(query).FirstOrDefault();
            UInt32 datatotal = 0;
            _user.OldData = _user.Data;
            datatotal = UInt32.Parse(_user.OldData);
            datatotal = datatotal * 10;
            _user.Data = datatotal.ToString();
            _collection.Save(_user);

        }

        private void button4_Click(object sender, EventArgs e)
        {
            _collection = _database.GetCollection<Station>("PowerCollection");
            IMongoQuery query = Query.EQ("Id_station", "1");
            Station _user = _collection.FindAs<Station>(query).FirstOrDefault();
            _user.OldData ="1";
            _user.Data = "1";
            _collection.Save(_user);
        }
    }
}
