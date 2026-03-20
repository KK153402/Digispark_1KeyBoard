# ATtiny85へのmicronucleusブートローダー書き込み手順（PlatformIO）

## はじめに

ATtiny85をDigisparkとして使用するためには、micronucleusブートローダーを書き込む必要があります。以前に作成した1キーボードもこちらの手法を使って、ブートローダーを書き込みました。

ArduinoIDEの書き込み方法は参考サイトに記載されています。今回はPlatformIOでの書き込み方法を記載してみました。間違いある可能性ありますので、ご容赦ください。

**参考サイト：**
- [USB接続で書き込めないDigispark ATtiny85のブートローダをアップデートして復活させる方法](https://dev.to/dariocasciato/attiny85-programming-your-complete-guide-with-arduino-and-platformio-37do)

---

## 1. Arduino UnoにArduinoISPを書き込む

Arduino Uno にArduinoISPスケッチを書き込む。これにより Arduino Uno がISPプログラマーとして機能するようになる。

PlatformIOで行う場合は、Arduino Uno用のプロジェクトを一時的に作成し、ArduinoISPコードを書き込む。コードは以下のGitHubから取得できる：

[https://github.com/DarioCasciato/PlatformIO-ATtiny85-Programming-with-Arduino-Nano](https://github.com/DarioCasciato/PlatformIO-ATtiny85-Programming-with-Arduino-Nano)

> **注意：** 書き込み装置としては「Arduino as ISP」の動作をする。「ArduinoISP」とは異なる。

---

## 2. Arduino UnoとATtiny85の接続

Arduino UnoとATtiny85を以下のように接続する。

| Arduino Uno | ATtiny85 |
|-------------|----------|
| 5V          | VCC (Pin 8) |
| GND         | GND (Pin 4) |
| Pin 13 (SCK) | Pin 7 (SCK) |
| Pin 12 (MISO) | Pin 6 (MISO) |
| Pin 11 (MOSI) | Pin 5 (MOSI) |
| Pin 10 (SS) | Pin 1 (RESET) |

---

## 3. PlatformIOプロジェクトの設定（テスト書き込み用）

VS Codeで新規プロジェクトを作成し、ATtiny85へのテスト書き込みを行う。この手順のおかげでAVRDUDEのパス情報を取得できる。

### 3-1. 新規プロジェクト作成

1. VS CodeのPlatformIOで「New Project」を選択
2. Boardで「ATtiny85」を検索し「Generic ATtiny85 (Atmel)」を選択
3. Frameworkは「Arduino」を選択
4. プロジェクトを作成する

### 3-2. platformio.ini の設定

プロジェクト生成後に `platformio.ini` を以下の内容に書き換える。`upload_port` の部分はArduino Unoが接続されているCOMポートに変更する。

```ini
[env:program_via_ArduinoISP]
platform = atmelavr
framework = arduino
board = attiny85
upload_protocol = stk500v1
upload_flags =
    -P$UPLOAD_PORT
    -b$UPLOAD_SPEED
upload_speed = 19200
upload_port = COM6
```

### 3-3. テスト書き込みの実行

`src/main.cpp` に適当なスケッチ（Blinkなど）を書き込み、PlatformIOで書き込みを実行する。書き込み成功後、PlatformIOの出力ログにAVRDUDEのパス情報が表示される。この情報を後の手順で使用するため、テキストエディタなどにコピーして保存しておく。

**実際に取得したコマンドラインの例（ArduinoIDEの場合）：**

```
"C:\Users\nessa\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17/bin/avrdude" "-CC:\Users\nessa\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17/etc/avrdude.conf" -v -V -pattiny85 -cstk500v1 -PCOM6 -b19200 "-Uflash:w:C:\Users\nessa\AppData\Local\arduino\sketches\667ABC3276A6A37F8F21A92B45C7283A/Blink.ino.hex:i"
```

PlatformIOで管理されているAVRDUDEのパスは以下のようなものが多い。実際のパスは環境に合わせて書き込みログから確認する。

```
C:\Users\[ユーザー名]\.platformio\packages\tool-avrdude\bin\avrdude.exe
C:\Users\[ユーザー名]\.platformio\packages\tool-avrdude\etc\avrdude.conf
```

---

## 4. コマンドプロンプトの開き方と使い方

以降の手順はコマンドプロンプトを開いてコマンドを実行する。

1. コマンドプロンプトが開いたら、コマンドをコピーして貼り付ける
2. 貼り付けは 右クリック → 貼り付け、または `Ctrl+V` で行う
3. `Enter` を押すとコマンドが実行される
4. 実行完了後は `C:\Users...>` のプロンプトが戻る

> **注意：** コマンドが長い場合でも途中で改行せず、1行で入力する必要がある。

---

## 5. ヒューズを読み取る

3で取得したコマンドラインの末尾の `-Uflash:w:...` の部分を書き換え、コマンドプロンプトで実行する。

**lfuse（低ヒューズ）を読み取る：**

```
"C:\Users\[ユーザー名]\.platformio\packages\tool-avrdude\bin\avrdude.exe" "-CC:\Users\[ユーザー名]\.platformio\packages\tool-avrdude\etc\avrdude.conf" -v -pattiny85 -cstk500v1 -PCOM6 -b19200 -U lfuse:r:-:i
```

**hfuse（高ヒューズ）と efuse（拡張ヒューズ）** も同様に末尾を書き換える：

```
... -U hfuse:r:-:i
... -U efuse:r:-:i
```

実行後、出力の中に以下のような行が表示される。期待される値は `E:FE`、`H:DF`、`L:F1` となる：

```
avrdude: safemode: Fuses OK (E:FE, H:DF, L:F1)
```

---

## 6. micronucleus firmwareのダウンロード

以下のGitHubから `t85_default.hex` をダウンロードする。

[https://github.com/micronucleus/micronucleus](https://github.com/micronucleus/micronucleus)

「firmware」フォルダ → 「releases」フォルダ内の `t85_default.hex` を使用する。upgrade版でも動作する場合がある。

---

## 7. micronucleusブートローダーの書き込み

コマンドプロンプトで以下のコマンドを実行する。パス情報と `t85_default.hex` の場所は環境に合わせて変更する。

```
"C:\Users\[ユーザー名]\.platformio\packages\tool-avrdude\bin\avrdude.exe" "-CC:\Users\[ユーザー名]\.platformio\packages\tool-avrdude\etc\avrdude.conf" -v -pattiny85 -cstk500v1 -PCOM6 -b19200 -Uflash:w:C:\Users\[ユーザー名]\Downloads\t85_default.hex:i -U lfuse:w:0xf1:m -U hfuse:w:0xdf:m -U efuse:w:0xfe:m
```

このコマンドで以下が同時に実行される：

- `t85_default.hex` のフラッシュ書き込み
- ヒューズビットの設定（lfuse=0xf1, hfuse=0xdf, efuse=0xfe）

以下のメッセージが表示されれば書き込み完了：

```
avrdude done.  Thank you.
```

---

## 8. 動作確認

1. ATtiny85をArduino Unoから取り外す
2. USBポートに直接接続する
3. 初回接続時はドライバーのインストールが必要になることがある
4. PlatformIOでDigisparkボード定義を追加し、プログラムを書き込めることを確認する

---

## ポイントとトラブルシューティング

| 症状 / 注意点 | 対処法 |
|--------------|--------|
| Digistump社のボード定義URL（digistump.com）にアクセス不能 | GitHubから直接micronucleusを取得する |
| 「Arduino as ISP」と「ArduinoISP」の混同 | 書込装置としては「Arduino as ISP」を選択する |
| USBが安定しない | クロック設定「Internal 16MHz (PLL)」でブートローダーを書き込む。PLLによりUSB通信（V-USB）に必要な約16MHzが得られる |
| `Expected signature for ATtiny25` エラー | ボード設定でATtiny85が正しく選択されているか確認する |

---

## まとめ

Digispark自作基板を使用する場合は、この手順が必須となります。以前に紹介した[1キーボード](./README.md)もこの手順でブートローダーを書き込んでいます。
