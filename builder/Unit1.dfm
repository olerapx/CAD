object Form1: TForm1
  Left = 216
  Top = 117
  Width = 972
  Height = 602
  Caption = #1048#1089#1089#1083#1077#1076#1086#1074#1072#1085#1080#1077' '#1075#1080#1087#1077#1088#1075#1088#1072#1092#1086#1074#1086#1081' '#1084#1086#1076#1077#1083#1080
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 964
    Height = 570
    ActivePage = TabSheet1
    Align = alClient
    TabIndex = 0
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = #1054#1076#1085#1086#1091#1088#1086#1074#1085#1077#1074#1086#1077' '#1088#1072#1079#1073#1080#1077#1085#1080#1077
      object Panel1: TPanel
        Left = 0
        Top = 0
        Width = 761
        Height = 542
        Align = alLeft
        TabOrder = 0
        object Chart1: TChart
          Left = 1
          Top = 1
          Width = 759
          Height = 256
          BackWall.Brush.Color = clWhite
          BackWall.Brush.Style = bsClear
          Title.Text.Strings = (
            #1047#1072#1090#1088#1072#1090#1099' '#1074' '#1096#1072#1075#1072#1093)
          View3D = False
          Align = alTop
          TabOrder = 0
          Visible = False
          object Series1: TBarSeries
            Marks.ArrowLength = 20
            Marks.Visible = True
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
        object Chart2: TChart
          Left = 1
          Top = 257
          Width = 759
          Height = 267
          BackWall.Brush.Color = clWhite
          BackWall.Brush.Style = bsClear
          Title.Text.Strings = (
            #1055#1088#1086#1094#1077#1085#1090' '#1074#1085#1077#1096#1085#1080#1093' '#1089#1074#1103#1079#1077#1081)
          View3D = False
          Align = alClient
          TabOrder = 1
          Visible = False
          object Series2: TBarSeries
            Marks.ArrowLength = 20
            Marks.Visible = True
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
        object ProgressBar1: TProgressBar
          Left = 1
          Top = 524
          Width = 759
          Height = 17
          Align = alBottom
          Min = 0
          Max = 100
          TabOrder = 2
        end
      end
      object Panel2: TPanel
        Left = 761
        Top = 0
        Width = 195
        Height = 542
        Align = alClient
        TabOrder = 1
        object Label1: TLabel
          Left = 24
          Top = 96
          Width = 76
          Height = 13
          Caption = #1063#1080#1089#1083#1086' '#1074#1077#1088#1096#1080#1085':'
        end
        object Label14: TLabel
          Left = 16
          Top = 328
          Width = 131
          Height = 13
          Caption = #1063#1080#1089#1083#1086' '#1087#1086#1076#1075#1088#1072#1092#1086#1074' '#1086#1090' 2 '#1076#1086':'
        end
        object Label2: TLabel
          Left = 24
          Top = 144
          Width = 90
          Height = 13
          Caption = #1063#1080#1089#1083#1086' '#1082#1086#1085#1090#1072#1082#1090#1086#1074':'
        end
        object Label3: TLabel
          Left = 24
          Top = 168
          Width = 20
          Height = 13
          Caption = 'Min:'
        end
        object Label4: TLabel
          Left = 24
          Top = 192
          Width = 23
          Height = 13
          Caption = 'Max:'
        end
        object Label5: TLabel
          Left = 24
          Top = 216
          Width = 89
          Height = 13
          Caption = #1052#1086#1097#1085#1086#1089#1090#1100' '#1088#1077#1073#1077#1088':'
        end
        object Label6: TLabel
          Left = 24
          Top = 240
          Width = 20
          Height = 13
          Caption = 'Min:'
        end
        object Label7: TLabel
          Left = 24
          Top = 264
          Width = 23
          Height = 13
          Caption = 'Max:'
        end
        object Label8: TLabel
          Left = 24
          Top = 16
          Width = 150
          Height = 13
          Caption = '--------------------------------------------------'
        end
        object Label9: TLabel
          Left = 24
          Top = 32
          Width = 128
          Height = 13
          Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1099' '#1080#1089#1093#1086#1076#1085#1099#1093' '#1043#1043':'
        end
        object Label10: TLabel
          Left = 24
          Top = 312
          Width = 150
          Height = 13
          Caption = '--------------------------------------------------'
        end
        object Label11: TLabel
          Left = 24
          Top = 48
          Width = 74
          Height = 13
          Caption = #1050#1086#1083#1080#1095#1077#1089#1090#1074#1086' '#1043#1043
        end
        object Button1: TButton
          Left = 40
          Top = 288
          Width = 105
          Height = 25
          Caption = #1057#1086#1079#1076#1072#1090#1100' '#1043#1043
          TabOrder = 0
          OnClick = Button1Click
        end
        object Button2: TButton
          Left = 8
          Top = 376
          Width = 81
          Height = 25
          Caption = #1056#1072#1085#1076#1086#1084
          Enabled = False
          TabOrder = 1
          OnClick = Button2Click
        end
        object Button3: TButton
          Left = 88
          Top = 376
          Width = 105
          Height = 25
          Caption = #1055#1086#1089#1083#1077#1076#1086#1074#1072#1090#1077#1083#1100#1085#1086
          Enabled = False
          TabOrder = 2
          OnClick = Button3Click
        end
        object Edit1: TEdit
          Left = 24
          Top = 112
          Width = 121
          Height = 21
          TabOrder = 3
          Text = '1000'
        end
        object Edit2: TEdit
          Left = 56
          Top = 160
          Width = 89
          Height = 21
          TabOrder = 4
          Text = '2'
        end
        object Edit3: TEdit
          Left = 56
          Top = 184
          Width = 89
          Height = 21
          TabOrder = 5
          Text = '5'
        end
        object Edit4: TEdit
          Left = 56
          Top = 232
          Width = 89
          Height = 21
          TabOrder = 6
          Text = '2'
        end
        object Edit5: TEdit
          Left = 56
          Top = 256
          Width = 89
          Height = 21
          TabOrder = 7
          Text = '7'
        end
        object Edit6: TEdit
          Left = 16
          Top = 344
          Width = 121
          Height = 21
          TabOrder = 8
          Text = '6'
        end
        object RadioGroup2: TRadioGroup
          Left = 16
          Top = 424
          Width = 169
          Height = 73
          Caption = #1057#1083#1086#1078#1085#1086#1089#1090#1100' '#1090#1088#1072#1089#1089#1080#1088#1086#1074#1082#1080':'
          ItemIndex = 0
          Items.Strings = (
            #1050#1074#1072#1076#1088#1072#1090#1080#1095#1085#1072#1103
            #1050#1091#1073#1080#1095#1077#1089#1082#1072#1103)
          TabOrder = 9
        end
        object Edit13: TEdit
          Left = 24
          Top = 64
          Width = 121
          Height = 21
          TabOrder = 10
          Text = '30'
        end
      end
    end
    object TabSheet2: TTabSheet
      Caption = #1048#1077#1088#1072#1088#1093#1080#1095#1077#1089#1082#1086#1077' '#1076#1080#1093#1086#1090#1086#1084#1080#1095#1077#1089#1082#1086#1077' '#1088#1072#1079#1073#1080#1077#1085#1080#1077
      ImageIndex = 1
      object Panel3: TPanel
        Left = 0
        Top = 0
        Width = 761
        Height = 542
        Align = alLeft
        TabOrder = 0
        object Label13: TLabel
          Left = 50
          Top = 64
          Width = 167
          Height = 67
          Caption = 'Label13'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -47
          Font.Name = 'Comic Sans MS'
          Font.Style = [fsItalic]
          ParentFont = False
          Visible = False
        end
        object ProgressBar2: TProgressBar
          Left = 1
          Top = 524
          Width = 759
          Height = 17
          Align = alBottom
          Min = 0
          Max = 100
          TabOrder = 0
        end
        object Chart3: TChart
          Left = 1
          Top = 1
          Width = 759
          Height = 256
          BackWall.Brush.Color = clWhite
          BackWall.Brush.Style = bsClear
          Title.Text.Strings = (
            #1047#1072#1090#1088#1072#1090#1099' '#1074' '#1096#1072#1075#1072#1093)
          View3D = False
          Align = alTop
          TabOrder = 1
          Visible = False
          object Series3: TBarSeries
            Marks.ArrowLength = 20
            Marks.Visible = True
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
        object Chart4: TChart
          Left = 1
          Top = 257
          Width = 759
          Height = 267
          BackWall.Brush.Color = clWhite
          BackWall.Brush.Style = bsClear
          Title.Text.Strings = (
            #1055#1088#1086#1094#1077#1085#1090' '#1074#1085#1077#1096#1085#1080#1093' '#1089#1074#1103#1079#1077#1081)
          View3D = False
          Align = alClient
          TabOrder = 2
          Visible = False
          object Series4: TBarSeries
            Marks.ArrowLength = 20
            Marks.Visible = True
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
      end
      object Panel4: TPanel
        Left = 761
        Top = 0
        Width = 195
        Height = 542
        Align = alClient
        TabOrder = 1
        object Label15: TLabel
          Left = 24
          Top = 96
          Width = 76
          Height = 13
          Caption = #1063#1080#1089#1083#1086' '#1074#1077#1088#1096#1080#1085':'
        end
        object Label20: TLabel
          Left = 24
          Top = 328
          Width = 79
          Height = 13
          Caption = #1063#1080#1089#1083#1086' '#1091#1088#1086#1074#1085#1077#1081':'
        end
        object Label21: TLabel
          Left = 24
          Top = 144
          Width = 90
          Height = 13
          Caption = #1063#1080#1089#1083#1086' '#1082#1086#1085#1090#1072#1082#1090#1086#1074':'
        end
        object Label22: TLabel
          Left = 24
          Top = 168
          Width = 20
          Height = 13
          Caption = 'Min:'
        end
        object Label23: TLabel
          Left = 24
          Top = 192
          Width = 23
          Height = 13
          Caption = 'Max:'
        end
        object Label24: TLabel
          Left = 24
          Top = 216
          Width = 89
          Height = 13
          Caption = #1052#1086#1097#1085#1086#1089#1090#1100' '#1088#1077#1073#1077#1088':'
        end
        object Label25: TLabel
          Left = 24
          Top = 240
          Width = 20
          Height = 13
          Caption = 'Min:'
        end
        object Label26: TLabel
          Left = 24
          Top = 264
          Width = 23
          Height = 13
          Caption = 'Max:'
        end
        object Label16: TLabel
          Left = 24
          Top = 16
          Width = 150
          Height = 13
          Caption = '--------------------------------------------------'
        end
        object Label17: TLabel
          Left = 24
          Top = 32
          Width = 129
          Height = 13
          Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1099' '#1080#1089#1093#1086#1076#1085#1086#1075#1086' '#1043#1043
        end
        object Label18: TLabel
          Left = 24
          Top = 304
          Width = 150
          Height = 13
          Caption = '--------------------------------------------------'
        end
        object Label12: TLabel
          Left = 24
          Top = 48
          Width = 74
          Height = 13
          Caption = #1050#1086#1083#1080#1095#1077#1089#1090#1074#1086' '#1043#1043
        end
        object Button4: TButton
          Left = 32
          Top = 384
          Width = 105
          Height = 25
          Caption = #1057#1090#1072#1088#1090
          TabOrder = 0
          OnClick = Button4Click
        end
        object Edit7: TEdit
          Left = 24
          Top = 112
          Width = 121
          Height = 21
          TabOrder = 1
          Text = '1000'
        end
        object Edit8: TEdit
          Left = 56
          Top = 160
          Width = 89
          Height = 21
          TabOrder = 2
          Text = '2'
        end
        object Edit9: TEdit
          Left = 56
          Top = 184
          Width = 89
          Height = 21
          TabOrder = 3
          Text = '5'
        end
        object Edit10: TEdit
          Left = 56
          Top = 232
          Width = 89
          Height = 21
          TabOrder = 4
          Text = '2'
        end
        object Edit11: TEdit
          Left = 56
          Top = 256
          Width = 89
          Height = 21
          TabOrder = 5
          Text = '7'
        end
        object Edit12: TEdit
          Left = 24
          Top = 344
          Width = 121
          Height = 21
          TabOrder = 6
          Text = '6'
        end
        object RadioGroup1: TRadioGroup
          Left = 16
          Top = 424
          Width = 169
          Height = 73
          Caption = #1057#1083#1086#1078#1085#1086#1089#1090#1100' '#1090#1088#1072#1089#1089#1080#1088#1086#1074#1082#1080':'
          ItemIndex = 0
          Items.Strings = (
            #1050#1074#1072#1076#1088#1072#1090#1080#1095#1085#1072#1103
            #1050#1091#1073#1080#1095#1077#1089#1082#1072#1103)
          TabOrder = 7
          OnClick = RadioGroup1Click
        end
        object Edit14: TEdit
          Left = 24
          Top = 64
          Width = 121
          Height = 21
          TabOrder = 8
          Text = '30'
        end
      end
    end
  end
end
