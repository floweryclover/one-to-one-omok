# OneToOneOmok
## 1대1 오목 예제

## 사용 라이브러리
* Windows Socket 2
* SDL2 (클라이언트 그래픽)

## 프로젝트 빌드
* CMake 변수 SDL_INCLUDE_DIR(/include), SDL_LIB_DIR(/lib/x64) 지정이 필요합니다.
  * Client/CMakeLists.txt에서 직접 수정하거나 CMake 명령어로 지정하세요.

## 사용법
Server.exe \<listen-address> \<listen-port> \<server-name>\
Client.exe \<server-address> \<server-port> \<your-name> - SDL 윈도우가 열립니다. 1대1 게임이므로 클라이언트 두개가 필요합니다.

## 설명
* 양측 모두 싱글스레드에서 동기 블로킹 IO 방식으로 작동합니다.
* 클라이언트의 경우 select()를 사용해서, 수신 블로킹 상태에서 화면 업데이트가 되지 않는 현상을 방지합니다.

## 현재까지 구현된 것
    * 접속 후 오목 게임 정상적으로 진행 가능

## 미구현
    * 장식적 요소들 (완전히 기본 그래픽 요소밖에 없음)
        * 내가 흑돌인지 백돌인지 표시해주기
        * 누구 턴인지 표시해주기
        * 게임 종료시 어디서 오목 완성되었는지 시각적으로 표시해주기...
        * 플레이어 이름 표시
    * 서버 이름 아직 어디에도 사용 안함
