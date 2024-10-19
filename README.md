## 1. 구현 동영상
### Youtube
[![구현 동영상](http://img.youtube.com/vi/_RFcu8tAdac/0.jpg)](https://www.youtube.com/watch?v=_RFcu8tAdac)
___

## 2. 플레이어 캐릭터 & 애니메이션

<img src="https://github.com/user-attachments/assets/a31009bb-bc0e-479e-ba8f-542b606fdc2e" width="600"/> 

### 관련 코드: [캐릭터 코드 보기](https://github.com/performeru/UERPG/tree/main/Source/RPG/Character)

## 3. 몬스터 & 애니메이션

<img src="https://github.com/user-attachments/assets/4a8d4ca5-e58c-46c3-91a2-e8368075e9de" width="600"/> 

### 관련 코드: [몬스터 코드 보기](https://github.com/performeru/UERPG/tree/main/Source/RPG/EnemyCharacter)

## 4. AI 
### 비헤이어 트리

<img src="https://github.com/user-attachments/assets/9b85553d-72a2-4b2d-b962-62cdbb58a030" width="600"/> 

### 관련 코드: [AI 코드 보기](https://github.com/performeru/UERPG/tree/main/Source/RPG/AI)

___

## 느끼점
언리얼을 처음 공부하고 구현을 해보려고 했을 때, 에셋을 구하는 것도 너무 어려웠다. 공부를 하면서 구현을 진행했고, 특히 애니메이션에서 몬스터의 죽음 애니메이션을 구현하는 게 어려웠다. 단순 구현만 하는 건 괜찮았는데, 공격 애니메이션과 연결했을 때 몬스터가 죽었을 시 죽음 애니메이션이 실행되어야 하는데, 계속 공격 애니메이션이 실행되는 바람에 해결하는 데 어려움을 겪었다. 해결법을 생각하던 중 bIsDead 변수를 만들어 이미 죽은 상태라면 추가 처리를 하지 않도록 하였고, 애니메이션 인스턴스에서 Montage_Play()를 통해 죽음 애니메이션을 재생할 때 StopAllMontages(0.0f)를 호출하여 모든 애니메이션을 멈추고 새로운 애니메이션을 시작하게 해서 문제를 해결했다.

구현할수록 코드가 길어지면서 주석의 중요성도 알게 되었다. 처음 구현을 시작하면서 주석을 쓰지 않고 로직을 만들고 구현하다 보니, 나중에는 왜 그렇게 구현했는지 알 수 없게 되어 뒤늦게 주석을 달기 시작했다.

설계 부분에서도 섹션을 지정해서 만드는 것이 좋겠다고 느꼈다. 섹션 없이 만들다 보니 주석이 없는 것처럼 어느 부분인지 헷갈리게 되었고 앞으로는 주석을 쓰는 습관을 들이고, 섹션도 만들어 설계를 더 깔끔하게 할 수 있도록 해야겠다.
