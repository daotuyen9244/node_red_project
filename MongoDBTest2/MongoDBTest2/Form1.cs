using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Configuration;

using CommonTools.TreeList;


using MongoDB.Bson;
using MongoDB.Driver;


using MongoDB.Driver.Builders;
using MongoDB.Driver.GridFS;
using MongoDB.Driver.Linq;

namespace MongoDBTest2
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
            _database = _server.GetDatabase(ConfigurationSettings.AppSettings["Database"], SafeMode.True);
        }

        public class Users1
        {
            public ObjectId Id { get; set; }

            public string Name { get; set; }

            public int Age { get; set; }

        }

        public class Users2
        {
            public ObjectId Id { get; set; }

            public string Name { get; set; }

            public string Location { get; set; }
        }

        public class Users3
        {
            public ObjectId Id { get; set; }

            public string Name { get; set; }

            public int Age { get; set; }

            public string Location { get; set; }
        }

        private void rbEntity1_CheckedChanged(object sender, EventArgs e)
        {
            txtName.Enabled = true;
            txtAge.Enabled = true;
            txtLocation.Enabled = true;
        }

        private void rbEntity2_CheckedChanged(object sender, EventArgs e)
        {
            txtName.Enabled = true;
            txtAge.Enabled = false;
            txtLocation.Enabled = true;
        }

        private void rbEntity3_CheckedChanged(object sender, EventArgs e)
        {
            txtName.Enabled = true;
            txtAge.Enabled = true;
            txtLocation.Enabled = false;
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            if (rbEntity1.Checked)
            {
                var _users = _database.GetCollection<Users3>("users");
                var user = new Users3 { };
                user.Age = Convert.ToInt32(txtAge.Text);
                user.Name = txtName.Text;
                user.Location = txtLocation.Text;
                _users.Insert(user);
                var id = user.Id;
            }
            else if (rbEntity2.Checked)
            {
                var _users = _database.GetCollection<Users2>("users");
                var user = new Users2 { };
                user.Name = txtName.Text;
                user.Location = txtLocation.Text;
                _users.Insert(user);
                var id = user.Id;
            }
            else if (rbEntity3.Checked)
            {
                var _users = _database.GetCollection<Users1>("users");
                var user = new Users1 { };
                user.Age = Convert.ToInt32(txtAge.Text);
                user.Name = txtName.Text;
                _users.Insert(user);
                var id = user.Id;
                
            }
            MessageBox.Show("User with name " + txtName.Text + " created");
        }

        private void btnFind_Click(object sender, EventArgs e)
        {
            _collection = _database.GetCollection<Users3>("users");
            IMongoQuery query = Query.EQ("Name", txtName.Text);
            Users3 _user = _collection.FindAs<Users3>(query).FirstOrDefault();
            if (_user != null)
            {
                MessageBox.Show(_user.Age.ToString());
            }
            else
            {
                MessageBox.Show("Not able to find any results with name " + txtName.Text);
            }
        }

        private void btnFindAll_Click(object sender, EventArgs e)
        {
            _collection = _database.GetCollection<Users3>("users");
            var userCursor = _collection.FindAllAs<Users3>();
            int count = 0;
            foreach (var item in userCursor)
            {
               
                //Update some values
                item.Age = 30;
                _collection.Save(item);

                //_collection.Remove(

                //TreeNode tNode = new TreeNode("(" + count.ToString() + ") {..}");
                //tvUsers_1.Nodes.Add(tNode);

                
                //CommonTools.Node tListNode1 = new CommonTools.Node("(" + count.ToString() + ") {..}");

                //CommonTools.Node tListNode2 = new CommonTools.Node("Test");
                //object[] array1 = new object[2];
                //array1[0]="fieldname0";
                //array1[1]="fieldname1";
                //tListNode2.SetData(array1);
                //tListNode1.Nodes.Add(tListNode2);
                //treeListView1.Nodes.Add(tListNode1);


                //MessageBox.Show(item.ToJson());
                count++;

                //TreeNode node2 = new TreeNode("C#");
                //TreeNode node3 = new TreeNode("VB.NET");
                //TreeNode[] array = new TreeNode[] { node2, node3 };
            }
        }
    }
}
