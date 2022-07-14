using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Drawing.Imaging;
using System.Windows.Forms;
using IronOcr;

namespace sudoku_cs
{
    public partial class Form1 : Form
    {
        public int[,] _Board = new int[9, 9];
        public Button[,] buttons = new Button[9, 9];
        public int button_sz = 100;
        private int __IMAGE_W = 625;
        private int __IMAGE_H = 625;
        private int __BOARD_POS_X = 172;
        private int __BOARD_POS_Y = 223;
        public Form1()
        {
            InitializeComponent();
            InitBoard();
        }

        private void InitBoard()
        {
            for(int i = 0; i < 9; i++)
            {
                for(int j = 0; j < 9; j++)
                {
                    Button button = new Button();
                    buttons[i, j] = button;
                    button.Size = new Size(button_sz, button_sz);
                    button.Text = _Board[i, j].ToString();
                    button.Font = new Font("Arial", 36F, FontStyle.Bold);
                    if (((i >= 0 && i <= 2) && (j >= 3 && j <= 5))||
                        ((i>=3&&i<=5)&&((j>=0&&j<=2)||(j>=6&&j<=8)))||
                        ((i>=6&&i<=8)&&(j>=3&&j<=5)))
                    {
                        button.BackColor = Color.FromArgb(180, 167, 218);
                    }
                    else
                    {
                        button.BackColor = Color.FromArgb(192, 192, 192);
                    }
                    
                    button.Location = new Point(j * button_sz, i * button_sz + 30);
                    this.Controls.Add(button);
                }
            }
        }

        private void setBoardRegionToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //TODO
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Bitmap capBitmap = new Bitmap(__IMAGE_W, __IMAGE_H, PixelFormat.Format32bppArgb);
            Rectangle capRec = Screen.AllScreens[0].Bounds;
            Graphics capGrap = Graphics.FromImage(capBitmap);
            capGrap.CopyFromScreen(__BOARD_POS_X, __BOARD_POS_Y, 0, 0, new Size(__IMAGE_W, __IMAGE_H));
            pictureBox1.Image = capBitmap;
            //capBitmap.Save("board.jpg", ImageFormat.Jpeg);

            GetNumberFromImage(capBitmap);
        }
        private void GetNumberFromImage(Bitmap img)
        {
            var Ocr = new IronTesseract();
            Ocr.Configuration.PageSegmentationMode = TesseractPageSegmentationMode.SingleChar;
            for(int i = 0; i < 9; i++)
            {
                for(int j = 0; j < 9; j++)
                {
                    int posx = 6 + j * 69;
                    int posy = 7 + i * 69;
                    var content = new System.Drawing.Rectangle() { X = posx, Y = posy, Width = 60, Height = 60 };
                    var Input = new OcrInput();
                    Input.AddImage(img, content);
                    var result = Ocr.Read(Input);

                    // update to screen
                    int n;
                    Boolean isNum = int.TryParse(result.Text, out n);
                    if(n < 1 || n > 9)
                    {
                        _Board[i, j] = 0;
                        buttons[i, j].Text = "0";
                    }
                    else
                    {
                        _Board[i, j] = n;
                        buttons[i, j].Text = n.ToString();
                    }
                }
            }
        }
    }
}
