object Form1: TForm1
  Left = 120
  Top = 98
  AutoScroll = False
  Caption = 'Form1'
  ClientHeight = 605
  ClientWidth = 841
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Image1: TImage
    Left = 8
    Top = 8
    Width = 265
    Height = 265
  end
  object Image2: TImage
    Left = 288
    Top = 8
    Width = 265
    Height = 265
  end
  object Image3: TImage
    Left = 568
    Top = 8
    Width = 265
    Height = 265
  end
  object Image4: TImage
    Left = 8
    Top = 487
    Width = 201
    Height = 202
    Visible = False
  end
  object Image5: TImage
    Left = 216
    Top = 280
    Width = 265
    Height = 265
  end
  object Label1: TLabel
    Left = 216
    Top = 536
    Width = 265
    Height = 20
    AutoSize = False
    Caption = 'loading'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 24
    Top = 432
    Width = 32
    Height = 13
    Caption = 'Label2'
  end
  object Label3: TLabel
    Left = 24
    Top = 464
    Width = 32
    Height = 13
    Caption = 'Label3'
  end
  object Button1: TButton
    Left = 8
    Top = 337
    Width = 201
    Height = 23
    Caption = '1 open image'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 8
    Top = 368
    Width = 201
    Height = 25
    Caption = '2 use function'
    TabOrder = 1
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 8
    Top = 400
    Width = 201
    Height = 25
    Caption = '3 MakeChain'
    TabOrder = 2
    OnClick = Button3Click
  end
  object Edit1: TEdit
    Left = 8
    Top = 280
    Width = 81
    Height = 21
    TabOrder = 3
    Text = '00'
  end
  object Chart1: TChart
    Left = 488
    Top = 464
    Width = 345
    Height = 89
    BackWall.Brush.Color = clWhite
    BackWall.Brush.Style = bsClear
    Title.Text.Strings = (
      'TChart')
    Title.Visible = False
    Legend.Visible = False
    View3D = False
    TabOrder = 4
    object Series1: TBarSeries
      Marks.ArrowLength = 20
      Marks.Visible = False
      SeriesColor = clRed
      XValues.DateTime = False
      XValues.Name = 'X'
      XValues.Multiplier = 1
      XValues.Order = loAscending
      YValues.DateTime = False
      YValues.Name = 'Bar'
      YValues.Multiplier = 1
      YValues.Order = loNone
    end
  end
  object Edit2: TEdit
    Left = 8
    Top = 312
    Width = 81
    Height = 21
    TabOrder = 5
    Text = '00'
  end
  object Chart2: TChart
    Left = 488
    Top = 376
    Width = 345
    Height = 89
    BackWall.Brush.Color = clWhite
    BackWall.Brush.Style = bsClear
    Title.Text.Strings = (
      'TChart')
    Title.Visible = False
    Legend.Visible = False
    View3D = False
    TabOrder = 6
    object Series2: TBarSeries
      Marks.ArrowLength = 20
      Marks.Visible = False
      SeriesColor = clRed
      XValues.DateTime = False
      XValues.Name = 'X'
      XValues.Multiplier = 1
      XValues.Order = loAscending
      YValues.DateTime = False
      YValues.Name = 'Bar'
      YValues.Multiplier = 1
      YValues.Order = loNone
    end
  end
  object Button4: TButton
    Left = 136
    Top = 496
    Width = 59
    Height = 25
    Caption = 'Load Font'
    TabOrder = 7
    OnClick = Button4Click
  end
  object ComboBox1: TComboBox
    Left = 24
    Top = 536
    Width = 169
    Height = 21
    ItemHeight = 13
    TabOrder = 8
    Text = 'ComboBox1'
  end
  object Chart3: TChart
    Left = 488
    Top = 288
    Width = 345
    Height = 92
    BackWall.Brush.Color = clWhite
    BackWall.Brush.Style = bsClear
    Title.Text.Strings = (
      'TChart')
    Title.Visible = False
    Legend.Visible = False
    View3D = False
    TabOrder = 9
    object Series3: TBarSeries
      Marks.ArrowLength = 8
      Marks.Visible = False
      SeriesColor = clRed
      XValues.DateTime = False
      XValues.Name = 'X'
      XValues.Multiplier = 1
      XValues.Order = loAscending
      YValues.DateTime = False
      YValues.Name = 'Bar'
      YValues.Multiplier = 1
      YValues.Order = loNone
    end
  end
  object Button5: TButton
    Left = 96
    Top = 312
    Width = 113
    Height = 25
    Caption = '5 skeleton'
    TabOrder = 10
    OnClick = Button5Click
  end
  object Button6: TButton
    Left = 96
    Top = 280
    Width = 113
    Height = 25
    Caption = '6 Adaptive Threshold'
    TabOrder = 11
    OnClick = Button6Click
  end
  object CheckBox1: TCheckBox
    Left = 24
    Top = 488
    Width = 97
    Height = 17
    Caption = 'Bold 1'
    TabOrder = 12
  end
  object CheckBox2: TCheckBox
    Left = 24
    Top = 512
    Width = 97
    Height = 17
    Caption = 'Italic 2'
    TabOrder = 13
  end
  object Edit3: TEdit
    Left = 80
    Top = 432
    Width = 121
    Height = 21
    TabOrder = 14
    Text = 'Time Usage (sec)'
  end
  object ProgressBar1: TProgressBar
    Left = 216
    Top = 560
    Width = 617
    Height = 17
    Min = 0
    Max = 100
    Smooth = True
    TabOrder = 15
  end
  object ProgressBar2: TProgressBar
    Left = 216
    Top = 584
    Width = 185
    Height = 17
    Min = 0
    Max = 100
    Smooth = True
    TabOrder = 16
  end
  object Memo1: TMemo
    Left = 584
    Top = 16
    Width = 225
    Height = 225
    ScrollBars = ssBoth
    TabOrder = 17
    WordWrap = False
  end
  object Button7: TButton
    Left = 624
    Top = 256
    Width = 97
    Height = 25
    Caption = '7 Save To File'
    TabOrder = 18
    OnClick = Button7Click
  end
  object Edit4: TEdit
    Left = 80
    Top = 464
    Width = 121
    Height = 21
    TabOrder = 19
    Text = 'Edit4'
  end
  object OpenPictureDialog1: TOpenPictureDialog
    Left = 88
    Top = 144
  end
  object SaveDialog1: TSaveDialog
    Filter = 'Text files (*.txt)|*.TXT'
    Left = 584
    Top = 248
  end
end
