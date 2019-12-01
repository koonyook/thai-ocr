object Form1: TForm1
  Left = 740
  Top = 205
  Width = 209
  Height = 421
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 24
    Width = 40
    Height = 13
    Caption = 'distance'
  end
  object Label2: TLabel
    Left = 8
    Top = 72
    Width = 39
    Height = 13
    Caption = 'insertion'
  end
  object Label3: TLabel
    Left = 8
    Top = 104
    Width = 46
    Height = 13
    Caption = 'deletetion'
  end
  object Label4: TLabel
    Left = 8
    Top = 136
    Width = 53
    Height = 13
    Caption = 'substitution'
  end
  object Label5: TLabel
    Left = 8
    Top = 168
    Width = 22
    Height = 13
    Caption = 'True'
  end
  object Label6: TLabel
    Left = 8
    Top = 256
    Width = 33
    Height = 13
    Caption = '% True'
  end
  object Label7: TLabel
    Left = 8
    Top = 200
    Width = 19
    Height = 13
    Caption = 'ALL'
  end
  object Button1: TButton
    Left = 8
    Top = 288
    Width = 89
    Height = 25
    Caption = '1 Check File'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 112
    Top = 288
    Width = 81
    Height = 25
    Caption = '2 Check String'
    TabOrder = 1
    OnClick = Button2Click
  end
  object Edit1: TEdit
    Left = 64
    Top = 16
    Width = 121
    Height = 21
    TabOrder = 2
    Text = 'Edit1'
  end
  object Edit2: TEdit
    Left = 64
    Top = 64
    Width = 121
    Height = 21
    TabOrder = 3
    Text = 'Edit2'
  end
  object Edit3: TEdit
    Left = 64
    Top = 96
    Width = 121
    Height = 21
    TabOrder = 4
    Text = 'Edit3'
  end
  object Memo1: TMemo
    Left = 112
    Top = 320
    Width = 81
    Height = 57
    Lines.Strings = (
      'Memo1')
    TabOrder = 5
    WordWrap = False
  end
  object Edit4: TEdit
    Left = 64
    Top = 128
    Width = 121
    Height = 21
    TabOrder = 6
    Text = 'Edit4'
  end
  object Edit5: TEdit
    Left = 64
    Top = 160
    Width = 121
    Height = 21
    TabOrder = 7
    Text = 'Edit5'
  end
  object Edit6: TEdit
    Left = 64
    Top = 192
    Width = 121
    Height = 21
    TabOrder = 8
    Text = 'Edit6'
  end
  object Edit7: TEdit
    Left = 64
    Top = 248
    Width = 121
    Height = 21
    TabOrder = 9
    Text = 'Edit7'
  end
  object Button3: TButton
    Left = 8
    Top = 320
    Width = 65
    Height = 25
    Caption = 'load model'
    TabOrder = 10
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 8
    Top = 352
    Width = 65
    Height = 25
    Caption = 'load answer'
    TabOrder = 11
    OnClick = Button4Click
  end
  object OpenDialog1: TOpenDialog
    Left = 72
    Top = 320
  end
  object OpenDialog2: TOpenDialog
    Left = 72
    Top = 352
  end
end
