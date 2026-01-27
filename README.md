# 칼퇴의 정석: VR 미스터리
![Unreal Engine 5](https://img.shields.io/badge/Unreal_Engine-5.6-black?logo=unrealengine)
![Language](https://img.shields.io/badge/Language-C%2B%2B_%26_BluePrint-blue?logo=cplusplus)
![Genre](https://img.shields.io/badge/Genre-Stealth_Action-red)
![Team](https://img.shields.io/badge/Team-3_People-green)
> **"당신의 야근은 의식이 된다."**

**Escape The Overtime: VR Mystery**는  
기존 PC 버전을 VR 환경으로 완벽하게 이식·확장한  
**오피스 오컬트 미스터리 VR 프로젝트**입니다.

플레이어는 단순히 퇴근을 목표로 하는 것이 아니라,  
직접 손으로 만지고, 몸을 숨기고, 단서를 조합하며  
**회사에 숨겨진 기이한 진실**을 파헤치게 됩니다.

---

## 📘 프로젝트 노션 (Notion)

기획 문서, 시스템 설계, 퍼즐 구조, 일정 관리 등  
본 프로젝트의 상세한 개발 기록은 아래 노션 페이지에서 확인할 수 있습니다.

👉 **[Escape The Overtime: VR Mystery 노션 바로가기](https://joyous-cupboard-1e9.notion.site/Escape-The-Overtime-VR-Mystery-2f179d39193e8156a406d317f0fa7ea2?source=copy_link)**

---
<img width="1170" height="704" alt="VR미스터리커버9" src="https://github.com/user-attachments/assets/5d5e6c01-10ee-46f4-9bb1-d6a8de238f55" />


## 📅 프로젝트 개요

- **개발 기간**: 2026.01.26 ~ 2026.03.20 (8주)
- **장르**: VR 1인칭 생존 호러 / 방탈출 / 미스터리
- **엔진**: Unreal Engine 5.6
- **타겟 플랫폼**
  - Meta Quest 2 / 3 (Android)
  - PC VR (SteamVR / Stove)
- **개발 인원**: 3명  
  *(기획/PM, 아트/레벨디자인, 프로그래밍)*

---

## 🎮 핵심 기능 (Key Features)

### 1. VR 물리 인터랙션 (Physics Interaction)

기존의 버튼 중심 인터랙션을  
**VR 핸드 트래킹 기반 물리 조작**으로 전면 재설계했습니다.

- **문 & 서랍**
  - 손잡이를 잡고(Grip) 돌리거나 당겨서 여는 실제 물리 조작
  - 문틈을 통해 내부를 엿보는 플레이 가능
- **물리 투척**
  - 머그잔, 서류철 등을 실제 투구 동작으로 던져 적을 타격하거나 유인
- **자판기 기믹**
  - 동전 투입 → 버튼 입력 → 음료 캔 따기까지 이어지는 세밀한 손동작 구현

---

### 2. 신체 활용 잠입 (Body Presence)

플레이어의 **실제 자세와 움직임**이 곧 게임 플레이가 됩니다.

- **Real Hiding**
  - 버튼 입력이 아닌 실제 행동 기반 은폐 시스템
  - 몸을 숙여 책상 밑으로 들어가거나 캐비닛 문을 직접 닫아야 은폐 판정
- **몰입형 이동 방식**
  - 텔레포트 이동
  - 스무스 로코모션(Smooth Locomotion) 옵션 지원

---

### 3. 미스터리 & 퍼즐 시스템 (Puzzle System)

단순 수집형 플레이를 배제하고  
**관찰·추리 중심의 방탈출 퍼즐**을 설계했습니다.

- **UV 라이트 퍼즐**
  - 육안으로 보이지 않는 핏자국, 기호, 비밀번호 단서 탐색
- **환경 퍼즐**
  - 특정 위치에 오브젝트(불상 등) 배치
  - 무게 조절을 통한 트리거 활성화
- **문서 조합**
  - 찢어진 문서를 양손으로 맞춰 완성하며 단서 획득

---

### 4. 연출 강화 (Enhanced Cinematic)

- **페이셜 캡처 (Live Link)**
  - iPhone Live Link를 활용
  - ‘부장님’ 캐릭터의 미세한 표정 변화와 광기를 실시간 반영
- **풀 보이스 더빙**
  - 전문 성우 녹음 적용
  - 스토리 몰입도 강화 및 플레이 가이드 제공

---

## 🛠️ 기술 스택 (Tech Stack)

### Engine & SDK

- **Unreal Engine 5.6**
  - VR Template Integration
- **Meta XR Plugin**
  - Meta Quest 기기 대응
  - 핸드 트래킹 지원
- **OpenXR**
  - 크로스 플랫폼 VR 호환성 확보

---

### Build Environment

- **IDE**: Visual Studio 2022
- **Android Studio**
  - Meta Quest(Android) 패키징을 위한 NDK / SDK 환경 구성

---

### Optimization Strategy

- **Forward Shading**
  - VR 환경에서 안정적인 프레임 확보
- **Asset Optimization**
  - Texture Atlas 활용
  - ISM(Instanced Static Mesh) 적용
  - Draw Call 최소화

---

## 📂 프로젝트 구조 (Directory Structure)

```text
EscapeTheOvertime_VR/
├── Config/             # VR 및 안드로이드 패키징 설정
├── Content/
│   ├── Blueprints/
│   │   ├── VR/         # VRPawn, MotionController, HandAnim
│   │   ├── Props/      # 물리 상호작용 오브젝트 (Door, Drawer, Mug)
│   │   └── System/     # GameInstance, PuzzleLogic
│   ├── Maps/           # 요일별 레벨 (Mon ~ Fri)
│   └── Audio/          # 성우 더빙 및 Spatial Audio 에셋
├── Source/
│   ├── EscapeTheOvertime/
│   │   ├── Public/     # C++ 헤더 (VRCharacter, InteractiveActor)
│   │   └── Private/    # C++ 소스 (물리 연산, 퍼즐 로직)
└── Saved/              # 로컬 로그 및 세이브 데이터


게임 내 등장하는 인물, 지명, 회사 등은 모두 허구이며 실제와 무관합니다.

광과민성 경고: 일부 챕터(금요일)에 강한 점멸 효과가 포함되어 있습니다.

Copyright © 2026 Team Overtime. All Rights Reserved.
