# Digispark 1KeyBoard

ATtiny85（Digispark）を使用した1キーキーボードです。

## 紹介動画

[![1キーキーボード作ってみた](https://img.youtube.com/vi/WLqFXZtJrVM/0.jpg)](https://youtu.be/WLqFXZtJrVM?si=OjhkI4Zj2ZjN5aRd)

## 概要

ATtiny85マイコンを搭載したDigisparkボードを使い、1キーのみのコンパクトなキーボードデバイスを作成しました。
3DモデリングからPCB設計、ファームウェア書き込みまで一から製作しています。

## 制作工程

| 時間 | 内容 |
|------|------|
| 0:42 | 3Dモデリング設計 |
| 1:33 | 回路図・PCB作成 |
| 2:59 | ブートローダー書き込み |
| 3:27 | 部品実装 |
| 3:45 | コードアップロード |
| 4:43 | 完成・動作確認 |

## 使用ツール・環境

- **マイコン:** ATtiny85（Digispark）
- **IDE:** Arduino IDE
- **3Dモデリング:** Fusion 360
- **基板設計:** KiCad（または同等のEDA）

## セットアップ

### 1. Digisparkのブートローダー書き込み

Digisparkのブートローダー書き込み手順は動画（2:59〜）または以下を参照してください。

### 2. Arduino IDEの設定

1. Arduino IDEにDigisparkのボードマネージャURLを追加
   ```
   http://digistump.com/package_digistump_index.json
   ```
2. ボードマネージャから **Digistump AVR Boards** をインストール
3. ボードを **Digispark (Default - 16.5mhz)** に設定

### 3. コードのアップロード

1. このリポジトリをクローン
   ```bash
   git clone https://github.com/YOUR_USERNAME/Digispark_1KeyBoard.git
   ```
2. Arduino IDEでスケッチを開く
3. **アップロードボタンを押してから** DigsiparkをUSBに接続

## ライセンス

MIT License
