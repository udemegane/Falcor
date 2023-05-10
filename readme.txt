# 提出プログラムの概要
このプログラムはDirectX12フレームワークである Falcor およびリアルタイムデノイザである Nvidia Real-Time Denoisers(NRD) を用い、以下の論文を元に作成されたリアルタイムパストレーシングデモになります。
- Spatiotemporal reservoir resampling for real-time ray tracing with dynamic direct lighting (Siggraph 2020)
    - https://benedikt-bitterli.me/restir/
- ReSTIR GI: Path Resampling for Real-Time Path Tracing (High Performance Graphics 2021)
    - https://research.nvidia.com/publication/2021-06_restir-gi-path-resampling-real-time-path-tracing

Falcor: https://github.com/NVIDIAGameWorks/Falcor
NRD:    https://developer.nvidia.com/rtx/ray-tracing/rt-denoisers

このプログラムは非常にGPU負荷が高いです。ハードウェアレイトレーシングに対応した現代的なGPUとWindows環境が必要です。


# プロジェクトページ(ソースコードおよびビルド済みバイナリ)
ファイルサイズが大きいので以下のページからダウンロードしてください。
URL: https://github.com/udemegane/Falcor/tree/ReSTIR_GI_Demo
ビルドする場合、@udemegane下にForkされたFalcorリポジトリの'ReSTIR_GI_Demo'ブランチからSubmoduleを含めてクローンする必要があります。
詳細はプロジェクトページのREADMEを参照してください。

## 注意:自作したコードとフレームワークの分かれ目について
自作コードは主に`Source/RenderPasses/Falcor-playground/`下のコードになります。
その他のcpp/shaderコードは基本的に全てFalcorフレームワークのものです。


# 実行環境等を含めた実行方法
URL: https://github.com/udemegane/Falcor/tree/ReSTIR_GI_Demo
プロジェクトページのREADMEに従い、ソースからビルドするか、ビルド済みバイナリをダウンロードしてください。

Demo.batでレンダラーが起動します。左上の"Files"から3DCGシーンを読み込んでください。
3DCGシーンは以下のリンクから私が使用していたものをダウンロードできます。
URL:

## 必要環境
RTX2060以上のハードウェアレイトレーシングに対応したGPU (Shader Model 6.5が必須です) とWindows10/11環境が必要です。

以下の環境で動作を確認しています。
- Env1
    Windows 11 22H2 (build 22621)
        Windows SDK 10.0.19041.0
        DirectX Graphics Tools
        DirectX12 Agility SDK
    NVIDIA Driver 517.0
    Visual Studio Community 2022 17.4.4 (for MSVC)
    Ryzen9 3900X / RTX2080

- Env2
    Windows 11 21H2 (build 22000)
        Windows SDK 10.0.19041.0
        DirectX Graphics Tools
        DirectX12 Agility SDK
    NVIDIA Driver 511.69
    Visual Studio Community 2022 17.4.3 (for MSVC)
    Core i9-9980HK / RTX2060




# プログラムを作成する上で苦労した箇所は？
簡潔にまとめると以下の点になります。
- Resampling時に適切な確率密度を算出すること
- 追加のbiasの導入時の計算ミスによって放射輝度が不適切な値に収束するのを防ぐこと
- フレームワークの知名度が低く、さらにドキュメントが更新されていないため仕様の理解とフレームワーク起因のエラー/バグ対処は全て自力で行う必要があること
- その他いろいろ

# 力をいれて作った部分で、「プログラム上」で特に注意してみてもらいたい箇所は？
基本的なアルゴリズムは元論文と同じだが、計算の省略のため幾つかの式変形、計算結果のキャッシュ、追加のbiasを導入して計算を効率化した点。




# 参考にしたソースファイルがあるなら、どの様なところを参考にしましたか？またその部分のファイル名を書いてください。
