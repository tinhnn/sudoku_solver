using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Windows.Forms;
using IronOcr;

namespace sudoku_cs
{
    public partial class Form1 : Form
    {
        public int[,] _Board_org = new int[9, 9];
        public int[,] _Board_comp = new int[9, 9];
        public Button[,] buttons = new Button[9, 9];
        public int button_sz = 100;
        private int __IMAGE_W = 625;
        private int __IMAGE_H = 625;
        private int __BOARD_POS_X = 172;
        private int __BOARD_POS_Y = 223;
        private Point pictureLocation;
        private Image picture;
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
                    button.Text = _Board_org[i, j].ToString();
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

        private void GetNumberFromImage(string img)
        {
            var Ocr = new IronTesseract();
            Ocr.Configuration.PageSegmentationMode = TesseractPageSegmentationMode.SingleChar;
            
            for (int i = 0; i < 9; i++)
            {
                for(int j = 0; j < 9; j++)
                {
                    int posx = 38 + j * 111 + (j / 3) * 4;
                    int posy = 386 + i * 111 + (i / 3) * 4;
                    var content = new System.Drawing.Rectangle() { X = posx, Y = posy, Width = 109, Height = 109 };
                    var Input = new OcrInput();
                    Input.AddImage(img, content);
                    Input.DeepCleanBackgroundNoise();
                    var result = Ocr.Read(Input);

                    // update to screen
                    int n;
                    Boolean isNum = int.TryParse(result.Text, out n);
                    if (n < 1 || n > 9)
                    {
                        _Board_comp[i, j] = _Board_org[i, j] = 0;
                        buttons[i, j].Text = "0";
                    }
                    else
                    {
                        _Board_comp[i, j] = _Board_org[i, j] = n;
                        buttons[i, j].Text = n.ToString();
                    }
                }
            }
        }
        Boolean isPresentInCol(int col, int num)
        { //check whether num is present in col or not
            for (int row = 0; row < 9; row++)
                if (_Board_comp[row, col] == num)
                    return true;
            return false;
        }
        Boolean isPresentInRow(int row, int num)
        { //check whether num is present in row or not
            for (int col = 0; col < 9; col++)
                if (_Board_comp[row, col] == num)
                    return true;
            return false;
        }
        Boolean isPresentInBox(int boxStartRow, int boxStartCol, int num)
        {
            //check whether num is present in 3x3 box or not
            for (int row = 0; row < 3; row++)
                for (int col = 0; col < 3; col++)
                    if (_Board_comp[row + boxStartRow, col + boxStartCol] == num)
                        return true;
            return false;
        }
        private Boolean isValidPlace(int row, int col, int num)
        {
            return !isPresentInRow(row, num) && !isPresentInCol(col, num) && !isPresentInBox(row - row % 3, col - col % 3, num);
        }
        bool findEmptyPlace(ref int row, ref int col)
        { //get empty location and update row and column
            for (row = 0; row < 9; row++)
                for (col = 0; col < 9; col++)
                    if (_Board_comp[row, col] == 0) //marked with 0 is empty
                        return true;
            return false;
        }

        private Boolean SudokuSolver()
        {
            int row=0, col=0;
            if (!findEmptyPlace(ref row, ref col))
                return true; //when all places are filled
            for (int num = 1; num <= 9; num++)
            { //valid numbers are 1 - 9
                if (isValidPlace(row, col, num))
                { //check validation, if yes, put the number in the grid
                    _Board_comp[row, col] = num;
                    if (SudokuSolver()) //recursively go for other rooms in the grid
                        return true;
                    _Board_comp[row, col] = 0; //turn to unassigned space when conditions are not satisfied
                }
            }
            return false;
        }

        private void Form1_DragEnter(object sender, DragEventArgs e)
        {
            e.Effect = DragDropEffects.Copy;
        }

        private void Form1_DragDrop(object sender, DragEventArgs e)
        {
            var data = e.Data.GetData(DataFormats.FileDrop);
            if(data != null)
            {
                var fileNames = data as string[];
                pictureBox1.Image = Image.FromFile(fileNames[0]);
                Stopwatch stopwatch = new Stopwatch();
                stopwatch.Start();
                GetNumberFromImage(fileNames[0]);
                stopwatch.Stop();
                TimeSpan stopwatchElapsed = stopwatch.Elapsed;
                Console.WriteLine(Convert.ToInt32(stopwatchElapsed.TotalMilliseconds));

                SudokuSolver();
                for(int i = 0; i < 9; i++)
                {
                    for(int j = 0; j < 9; j++)
                    {
                        buttons[i, j].Text = _Board_comp[i, j].ToString();
                        if (_Board_comp[i,j] != _Board_org[i, j])
                        {
                            buttons[i, j].ForeColor = Color.FromArgb(255, 0, 0);
                        }
                        else
                        {
                            buttons[i, j].ForeColor = Color.FromArgb(0, 0, 0);
                        }
                    }
                }
            }
        }
    }
}
