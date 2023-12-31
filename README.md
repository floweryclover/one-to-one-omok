# OneToOneOmok - 1대1 오목 예제
![이미지](https://github.com/floweryclover/one-to-one-omok/assets/35604150/23451bd4-e21a-4d4d-8a1a-350f2048630f)
* 1 서버, 2 클라이언트 구조의 TCP 소켓 프로그래밍 예제입니다.
* C언어로 Windows Socket의 저수준 API만을 이용하여 구현했습니다.
* 직접 오목 프로토콜을 구현하여 패킷 기반으로 통신합니다.
  * Common/을 참고하세요.
  * 각각 4byte의 바디길이+4byte의 패킷유형을 의미하는 총 8바이트 헤더 부분과, 가변 길이 바디 부분으로 구성되어 있습니다.
  * 새로운 패킷을 정의하거나 기존 패킷을 수정할때도 간단하게 가능합니다.
* 싱글 스레드 모델에서, 블로킹 동기 방식으로 통신합니다.
  * 클라이언트에서, 수신 대기 중에도 화면을 업데이트 하기 위해 select() 함수를 사용했습니다.
  
## 사용 라이브러리
* Windows Socket 2
* SDL2 (클라이언트 그래픽)

## 프로젝트 빌드
* CMake 변수 SDL_INCLUDE_DIR(/include), SDL_LIB_DIR(/lib/x64) 지정이 필요합니다.
  * Client/CMakeLists.txt에서 직접 수정하거나 CMake 명령어로 지정하세요.

## 사용법
Server.exe \<listen-address> \<listen-port> \<server-name>\
Client.exe \<server-address> \<server-port> \<your-name> - SDL 윈도우가 열립니다. 1대1 게임이므로 클라이언트 두개가 필요합니다.

## 현재까지 구현된 것
    * 접속 후 오목 게임 정상적으로 진행 가능
    * 패킷 기반 통신

## 미구현
    * 장식적 요소들 (완전히 기본 그래픽 요소밖에 없음)
        * 내가 흑돌인지 백돌인지 표시해주기
        * 누구 턴인지 표시해주기
        * 게임 종료시 어디서 오목 완성되었는지 시각적으로 표시해주기...
        * 플레이어 이름 표시
    * 서버 이름 아직 어디에도 사용 안함

## 추후 목표
  * 서버가 여러 개의 게임을 동시에 진행하게 만들기
