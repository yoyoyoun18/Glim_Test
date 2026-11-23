# ImageProcessor 과제 제출

## 과제 개요

24비트 BMP 이미지를 읽어 필터를 적용하고, 결과를 다시 BMP로 저장하는 CLI 프로그램입니다.

- 외부 라이브러리(OpenCV 등) 없이 **C++ 표준 라이브러리(STL)만** 사용합니다.
- BMP 입출력(`BmpParser`), 픽셀 버퍼(`ImageBuffer`), CLI 인자 처리(`CommandLineParser`)는 제공된 스타터 코드를 사용합니다.
- 이미지 처리 알고리즘은 직접 구현합니다.

## 개발 환경

| 항목 | 내용 |
|---|---|
| OS | Windows 11 |
| IDE | Visual Studio 2022 (플랫폼 도구 집합 v143) |
| 언어 표준 | C++17 |
| 컴파일 옵션 | 경고 수준 `/W4`, 소스 인코딩 `/utf-8` |
| 외부 라이브러리 | 없음 (STL만 사용) |

## 빌드 방법

### Visual Studio

1. `ImageProcessor.sln`을 엽니다.
2. 구성을 **Release**, 플랫폼을 **x64**로 선택합니다.
3. **빌드 → 솔루션 빌드** (`Ctrl+Shift+B`)

### 명령줄 (MSBuild)

Visual Studio의 **Developer PowerShell**(또는 Developer Command Prompt)에서 실행합니다.

```powershell
msbuild ImageProcessor.sln /p:Configuration=Release /p:Platform=x64
```

빌드 결과물은 `.\x64\Release\ImageProcessor.exe`에 생성됩니다. 아래 실행 명령어는 모두 솔루션 폴더 기준입니다.

## 구현 항목

### 필수 항목

- [x] BMP 파일 입출력 (`BmpParser` 활용, 24비트 BMP)
- [x] CLI 인자 처리 (`--input`, `--output`, `--filter` 등)
- [x] 이미지 처리 알고리즘 2개 이상 직접 구현
- [x] 처리 결과를 BMP 파일로 저장

### 기본 요구사항

- [x] Grayscale 변환 (RGB 가중치 공식)
- [x] 밝기/대비 조절 (0~255 클램핑)
- [x] 이진화 (임계값 파라미터화)
- [ ] 3×3 Convolution 필터 (Blur, Sharpen)
- [x] 히스토그램 분석 (픽셀 분포 계산 후 콘솔 출력)
- [x] 이미지 크롭 / 리사이즈 (Nearest)
- [x] 상하 / 좌우 반전

### 고급 요구사항

- [x] 필터 파이프라인 체인 (`--pipeline`)
- [x] 추상 클래스 기반 설계 (`FilterBase` 상속, `FilterFactory` 등록)
- [ ] 멀티스레드 병렬 처리
- [x] 로그 파일 출력 (처리 시간, 필터 파라미터, 성공/실패)

## 실행 방법

### 사용법

```powershell
.\x64\Release\ImageProcessor.exe --input <입력.bmp> --output <출력.bmp> --filter <필터>
.\x64\Release\ImageProcessor.exe --input <입력.bmp> --output <출력.bmp> --pipeline "<필터>, <필터>, ..."
```

| 옵션 | 설명 |
|---|---|
| `-i`, `--input` | 입력 BMP 파일 (24비트) |
| `-o`, `--output` | 출력 BMP 파일 |
| `-f`, `--filter` | 적용할 필터 하나 |
| `-p`, `--pipeline` | 순서대로 적용할 필터 목록 (`,` 로 구분) |
| `-h`, `--help` | 도움말 |

`--filter` 와 `--pipeline` 중 하나만 사용합니다.

### 필터 목록

필터 파라미터는 이름 뒤에 `:` 로 붙이거나, 별도 옵션으로 줄 수 있습니다.

| 필터 | 파라미터 (`:` 형식) | 파라미터 (옵션 형식) |
|---|---|---|
| `grayscale` | - | - |
| `threshold` | `threshold:128` | `-t 128` (기본 128) |
| `brightness` | `brightness:20:1.2` (밝기:대비) | `-b 20 -c 1.2` (기본 0, 1.0) |
| `crop` | `crop:10:20:100:80` (x:y:너비:높이) | `--x 10 --y 20 --width 100 --height 80` |
| `resize` | `resize:200:150` (너비:높이) | `--width 200 --height 150` |
| `flip:h` / `flip:v` | 좌우 / 상하 반전 | - |
| `histogram` | - (콘솔에 출력, 이미지는 그대로) | - |

### 실행 명령어 (예시)

```powershell
# Grayscale 변환
.\x64\Release\ImageProcessor.exe --input .\Resource\1_astronaut.bmp --output .\Resource\1_astronaut_grayscale.bmp --filter grayscale
```

```powershell
# 이진화 (임계값 100)
.\x64\Release\ImageProcessor.exe --input .\Resource\4_text_page.bmp --output .\Resource\4_text_page_threshold.bmp --filter threshold:100
```

```powershell
# 밝기/대비 조절
.\x64\Release\ImageProcessor.exe --input .\Resource\2_coffee.bmp --output .\Resource\2_coffee_brightness.bmp --filter brightness -b 30 -c 1.2
```

```powershell
# 크롭 / 리사이즈
.\x64\Release\ImageProcessor.exe --input .\Resource\3_chelsea_cat.bmp --output .\Resource\3_chelsea_cat_crop.bmp --filter crop:100:50:200:150
.\x64\Release\ImageProcessor.exe --input .\Resource\3_chelsea_cat.bmp --output .\Resource\3_chelsea_cat_resize.bmp --filter resize:200:150
```

```powershell
# 좌우 / 상하 반전
.\x64\Release\ImageProcessor.exe --input .\Resource\5_checkerboard.bmp --output .\Resource\5_checkerboard_flip_h.bmp --filter flip:h
.\x64\Release\ImageProcessor.exe --input .\Resource\5_checkerboard.bmp --output .\Resource\5_checkerboard_flip_v.bmp --filter flip:v
```

```powershell
# 히스토그램 출력
.\x64\Release\ImageProcessor.exe --input .\Resource\3_chelsea_cat.bmp --output .\Resource\3_chelsea_cat_histogram.bmp --filter histogram
```

```powershell
# 필터 파이프라인 체인(고급)
.\x64\Release\ImageProcessor.exe --input .\Resource\3_chelsea_cat.bmp --output .\Resource\3_chelsea_cat_pipeline.bmp --pipeline "grayscale, brightness:20:1.2, threshold:128"
```

### 로그

실행할 때마다 실행한 폴더 기준 `Log\<날짜>\ImageProcessingLog.csv` 에 처리 기록이 남습니다.
