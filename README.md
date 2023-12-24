# OneToOneOmok
## 1대1 오목 예제

* Windows Socket 2
* 싱글스레드 모델 (양측 모두)
    * 서버는 블로킹 동기 IO (오목게임이라 턴이 명확)
    * 클라이언트는 논블로킹 동기 IO (블로킹 중에도 그래픽 업데이트를 위해서)

* 현재까지 구현된 것
    * 접속 후 오목 게임 정상적으로 진행 가능

* 미구현
    * 장식적 요소들 (완전히 기본 그래픽 요소밖에 없음)
        * 내가 흑돌인지 백돌인지 표시해주기
        * 누구 턴인지 표시해주기
        * 게임 종료시 어디서 오목 완성되었는지 시각적으로 표시해주기...