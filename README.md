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

- 
- 
- 
- 

## 실행 명령어 (예시)

<!-- 예시 입니다. 기존 예시를 지우고 구현한 항목에 맞게 명령어를 작성해주세요. -->

```powershell
# Grayscale 변환
.\x64\Release\ImageProcessor.exe --input .\Resource\1_astronaut.bmp --output .\Resource\1_astronaut_grayscale.bmp --filter grayscale
```

```powershell
# Blur 처리 + 임계값 지정
.\x64\Release\ImageProcessor.exe --input .\Resource\2_coffee.bmp --output .\Resource\2_coffee_blur_threshold.bmp --filter blur --threshold 128
```

```powershell
# 필터 파이프라인 체인(고급)
.\x64\Release\ImageProcessor.exe --input .\Resource\3_chelsea_cat.bmp --output .\Resource\3_chelsea_cat_pipeline.bmp --pipeline "grayscale, blur, threshold:128"
```
