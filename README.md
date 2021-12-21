# FreeD Protocol Data Delay Receive 시스템

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Camera Tracker에서 Tracking 정보를 UDP를 통해 FreeD Protocol Data로 전달 받을 때 영상 데이터와 Tracking 정보인 FreeD Data가 동기화 되지 못하고 영상 데이터가 조금 늦게 전달 될 수가 있다.

이때 Tracking 정보를 영상 데이터가 도착하는 시점까지 늦춰 영상 데이터가 도착하게 되면 그때부터 동기화 시킬 수 있는 시스템이 필요하게 되어 UE4용 Plugin을 개발 하였다.

## 사용법

FreeD Protocol Data Delay Receive 시스템 테스트를 위해서 2개의 Actor Blueprint가 필요하다.

![](https://github.com/Devcoder-IndieWorks/FreeDReceivePluginSample/blob/master/Images/Test_Blueprint_Actors.png)

BP_FreeDSendActor는 AFreeDSendActor 클래스를 기반으로 만들어진 Blueprint로 테스트용으로 입력된 FreeD Data를 지정된 간격으로 BP_FreeDReceiverActor에게 전달하는 역할을 한다.

BP_FreeDReceiveActor는 AVFreeDReceiveActor 클래스를 기반으로 만들어진 Blueprint로 BP_FreeDSendActor로 부터 전달 받은 테스트용 FreeD Data를 설정된 시간동안 버퍼에 쌓아 둔 후 이후 일정한 간격으로 하나씩 출력 시키는 역할을 한다.

![](https://github.com/Devcoder-IndieWorks/FreeDReceivePluginSample/blob/master/Images/Test_FreeD_Datas.png)

### 테스트 구성

테스트 구성은 기본맵에 BP_FreeDSendActor와 BP_FreeDReceiveActor를 배치하고, Level Blueprint에 테스트용 로직을 구현한다.

우선 BP_FreeDSendActor에서 테스트용 FreeD Data 전달을 시작하기 위해 **키보드 이벤트 1**에서 StartSend() 함수가 호출 되도록 처리 해 준다. StartSend() 함수는 매개변수로 테스트용 FreeD Data를 받을 AVFreeDReceiveActor 계열 객체 참조가 필요하다.

그리고 BP_FreeDSendActor에서 테스트용 FreeD Data을 더 이상 전달 하지 않도록 하기 위해 **키보드 이벤트 2**에서 StopSend() 함수가 호출 되도록 처리해 준다.

![](https://github.com/Devcoder-IndieWorks/FreeDReceivePluginSample/blob/master/Images/Test_BP_FreeDSendActor_Code.png)

Level Blueprint **BeginPlay 이벤트**에서 BP_FreeDReceiveActor가 BP_FreeDSendActor로부터 전달 받은 하나의 테스트용 FreeD Data를 어떤식으로 처리 할지에 대한 구체적인 구현을 정의한 함수나 커스텀 이벤트를 OnFreeDReceiveMessageDelegate에 연결한다.

![](https://github.com/Devcoder-IndieWorks/FreeDReceivePluginSample/blob/master/Images/Test_BP_FreeDReceiveActor_Code.png)

BP_FreeDSendActor의 Property인 Interval을 설정하여 설정된 간격으로 테스트용 FreeD Data를 BP_FreeDReceiveActor에 전달 한다.

![](https://github.com/Devcoder-IndieWorks/FreeDReceivePluginSample/blob/master/Images/BP_FreeDSendActor_Set_Property.png)

BP_FreeDReceiveActor에서는 최초 일정 시간동안 대기를 위한 시간을 Delay에 설정 해 주고, 이후 일정 간격으로 BP_FreeDSendActor에서 받은 테스트용 FreeD Data를 처리할 시간을 Interval에 설정 해 준다. 추가로 UE4 Editor의 Log창에 FreeD Protocol Data Delay Receive 시스템의 로그 정보를 표시 할 지 여부를 설정 하는 Output Log 항목도 있다.

![](https://github.com/Devcoder-IndieWorks/FreeDReceivePluginSample/blob/master/Images/BP_FreeDReceiveActor_Set_Property.png)

### Delay 작동 방식

BP_FreeDSendActor에서 StartSend() 함수를 실행하면 FreeDatas에 저장되어 있는 테스트용 FreeD Data를 UE4 WorldTimer를 이용하여 Interval 간격으로 BP_FreeDReceiveActor에게 보낸다.

![](https://github.com/Devcoder-IndieWorks/FreeDReceivePluginSample/blob/master/Images/AFreeDSendActor_StartSend.png)

(1)번이 WorldTimer를 설정하는 부분이고, (2)번이 Interval 간격으로 FreeD Data를 BP_FreeDReceiveActor에서 전달 하는 부분이다.

![](https://github.com/Devcoder-IndieWorks/FreeDReceivePluginSample/blob/master/Images/AVFreeDReceiveActor_ReceiveMessage.png)

BP_FreeDReceiveActor는 ReceiveMessage() 함수를 통해 전달 받은 테스트용 FreeD Data를 ReceiveQueue에 저장한다.

BP_FreeDSendActor의 StartSend() 함수에서  (3)번의 StartReceive() 함수는 최초 일정 시간 Delay와 Delay 후 Interval 간격으로 처리하기 위한 Co Routine 기능이 실행 되도록 한다.

![](https://github.com/Devcoder-IndieWorks/FreeDReceivePluginSample/blob/master/Images/AVFreeDReceiveActor_StartReceive.png)

Delay와 Interval 간격 동안 처리를 담당하는 Co Routine을 설정하는 부분은 BP_FreeDReceiveActor의 BeginPlay() 함수에서 한다.

![](https://github.com/Devcoder-IndieWorks/FreeDReceivePluginSample/blob/master/Images/AVFreeDReceiveActor_Begin.png)

Co Routine 설정은 C++ Lambda 함수로 설정한다.

(1)번 Lambda 함수는 BP_FreeDSendActor로부터 전달 받은 테스트용 FreeD Data를 저장한 Queue에서 Interval 간격으로 처리하는 곳에서 사용하는 Queue로 데이터를 매프레임 하나씩 옮기는 처리를 한다.

(2)번 Lambda 함수는 Interval마다 실행 되는 부분으로 (1)에서 옮겨져 저장된 Queue(IntermediateQueue)에서 하나씩 가져와 Blueprint에서 설정한 Delegate에 전달하여 처리 되도록 한다.

(3)번 Lambda 함수는 (2)번의 매개변수인 elapsedTime을 계산하기 위해 UE4의 현재 시간(초단위)을 얻어오는 함수를 호출 한다.

(4)번은 (1) ~ (3)  내용으로 설정되어 생성된 Co Routine을 실행하기 위해 UE4의 CoreTicker에 추가한다.

마지막으로 BP_FreeDSendActor의 StopSend() 함수를 호출하여 WorldTimer를 제거하면 테스트용 FreeD Data 전달이 중단 된다.

![](https://github.com/Devcoder-IndieWorks/FreeDReceivePluginSample/blob/master/Images/AFreeDSendActor_StopSend.png)

## Delay 처리를 위한 Co Routine System 구현

엄밀히 말하면 Co Routine과 비슷하게 동작하도록 C 프로그래밍의 트릭을 이용하여 구현하였다. Co Routine 처럼 내부 스택 정보를 보관하지 않으며, 단지 함수 내 특정 부분에서 함수를 빠져 나올 수 있으며, 다시 함수에 진입시 이전에 함수에서 빠져 나온 부분에서부터 처리가 되도록 하는 것만 같다고 할 수 있다. 이런 점 때문에 Co Routine이라고는 하였지만, 정확하게는 Co Routine은 아닌것이다.

그럼 함수에서 임의의 위치에서 리턴되고, 다시 함수에 진입시 이전에 리턴된 부분부터 함수가 처리 되도록 하는 방법에 대해 설명한다.

Co Routine처럼 작동하는 기능을 구현하기 위해 **FVCoroutineBase** 클래스와 **FVCoroutineRef** 클래스를 만든다.

![](https://github.com/Devcoder-IndieWorks/FreeDReceivePluginSample/blob/master/Images/Coroutine_Classes.png)

**FVCoroutineBase**는 Co Routine처럼 작동할 수 있는 기능을 구현 할 수 있도록 해주기 위한 기반 클래스로써 나중에 다시 설명하겠지만, 가상 함수인 Run() 함수를 재정의해서 그곳에 Co Routine처럼 작동하도록 하는 코드를 구현 할 수 있도록 해준다.

**FVCoroutineRef**는 Co Routine에서 함수의 임의의 지점에서 리턴 되고, 다시 함수에 진입시 이전에 리턴된 지점에서 시작 할 수 있도록 하기 위한 리턴 지점에 대한 정보와 Co Routine에서 함수 재진입 시간을 설정해서 처리 하는 것과 같은 역할을 하는 기능에 필요한 대기 시간에 대한 정보를 저장하는 기능을 담당한다.

C 프로그래밍의 매크로 기법은 트릭을 사용 할 수 있도록 해주는 도구로써 장점을 가지고 있다. 물론 장정보다 단점이 많긴 하지만 어떻게 사용하는지에 따라서 장점으로도 사용 할 수 있기 때문이다.

![](https://github.com/Devcoder-IndieWorks/FreeDReceivePluginSample/blob/master/Images/Coroutine_Macro.png)

Co Routine처럼 함수에서 임의의 위치에서 리턴 되고 다시 진입시 리턴된 부분부터 처리 되도록 하기 위해서는 리턴될 때 리턴 된 지점을 저장해 두어야 한다. 코드의 라인 위치를 알아 내는 매크로가 **__LINE__** 매크로이다. 이 매크로를 이용해서 리턴 된 지점을 저장하는 기능을 구현 할 수 있다. 또한 C 프로그래밍의 switch 구문의 특징을 함께 이용해서 재진입시 이전에 리턴된 지점에서 시작할 수 있도록 처리한다.

### Macro들에 대해

**VCoroutineReEnter** : 함수 진입시 이전에 리턴된 지점으로 점프해서 처리 되도록 하는 역할을 한다. switch 구문을 이용하고  FVCoroutineRef 인스턴스를 생성한다. FVCoroutineRef 인스턴스는 uint32 타입으로 형변환 할 수 있는 operator 함수를 가지고 있으며 이 형변환은 이전에 리턴된 지점에 대한 코드 라인 정보로 변환된다.(최초 진입시에는 0으로 변환된다.)

**VCoroutineEntry** : 최초 Co Routine 진입시에 대한 처리 및 임의의 지점에서 리턴하는 처리를 한다. Co Routine이 적용된 함수가 처음으로 호출되어 처리되면 **FVCoroutineRef인 CoRef**는 0으로 변환되고 매크로에 정의된 **case 0:** 구문을 호출하게 된다. 이후 다시 함수에 진입시에는 더 이상 **case 0:** 구문은 호출되지 않는다. 그리고 함수의 임의의 지점에서 리턴하는 코드는 **BailOutOfCoroutine:**에서 처리하는데, **C의 goto 문**으로 처리된다. 매크로의 이름처럼 최초 시작시에 대한 처리와 리턴에 대한 처리를 전담하는 매크로이다.

**VCoroutineBegin** : Co Routine 구문 시작에 대한 코드 정의를 위한 매크로이다. for문을 이용해서 한번만 매크로 다음에 정의한 구문이 호출되게 하고, 그 다음은 매크로가 정의된 함수에서 리턴할 수 있도록 **goto BailOutOfCoroutine**를 실행한다.

**VCoroutineEnd** : VCoroutineBegin 매크로 쌍으로 사용하는 매크로이다. 내용은 특별할 것은 없고,  VCoroutineBegin 매크로에서 정의한 for문의 닫을 **}**를 정의한다.

**VCoroutineNull( wait )/VCoroutineStatement( wait, statement )** : VCoroutineNull 매크로는 시간 설정에 대한 wait 구문을 실행하고 함수에서 리턴하는 코드를 적용한다. VCoroutineStatement 매크로는 시간 설정에 대한 wait 구문뿐만 아니라, 다른 상태처리에 대한 구문을 실행하고 함수에서 리턴하는 코드를 적용한다. 이 두 매크로는 직접으로 사용하기보다는 다음에 설명하는 매크로들에서 공통으로 사용하는 코드를 위해 사용된다.

**VCoYieldNullBegin** : VCoroutineReEnter 매크로의 switch에서 CoRef의 uint32 operator 함수의 코드 라인 번호 평가 따라 선택될 case 문을 정의한다. Co Routine이 작동하는 방식을 생각해 보면, VCoYieldNullBegin 매크로 만나게 되면 **CoRef에 현재 코드 라인**을 설정 해 주고,  매크로 다음에 정의한 구문을 처리할 것이다. 그 후 임의의 지점에서 리턴한 후 다시 진입시 if문안에 정의한 **case __LINE__:** 으로 건너띄어 호출되고 함수의 다음 부분부터 실행하게 된다. 이러한 프로그래밍 트릭은 C의 switch문의 특징(?)을 이용한 프로그래밍 방식이다.

**VCoYieldNullEnd** : VCoroutineEnd와 마찬가지로 닫을 **}**를 정의한다.

**VCoYieldReturnNull/VCoYieldReturnNullStatement( statement )** : 위에서 정의한 VCoroutineBegin/VCoroutineEnd 매크로 쌍을 이용하여 이 매크로가 선언된 함수 내 위치에서 이전까지 구현된 내용을 한번 처리하고, 매크로가 선언된 위치에서 리턴하는 역할을 한다. 그리고 다시 함수에 재진입시 VCoroutineReEnter 매크로가 선언된 위치에서부터 이 매크로 선언된 위치에 구현된 내용은 건너띄고, 이후 내용부터 처리되게 한다. 이 매크로는 대기 시간 없이 한번은 구현내용이 처리 되고, 한번은 리턴하는 식으로 작동한다.

**VCoYieldWaitBegin/VCoYieldReturnWait( wait )/VCoYieldReturnWaitStatement( wait, statement )** : 위에 정의한 매크로 기능에 일정시간 동안 대기를 하도록 하는 기능을 추가한 매크로이다. CoRef.HasWaiting() 함수를 통해  지정된 시간동안 매크로가 선언된 위치 이후에 구현된 내용을 처리 하지 못하도록 막는 역할을 한다.

### Delay 처리를 위한 기반 클래스 구현

Co Routine관련 매크로들을 이용하여 Delay 처리를 하기 위한 클래스를 구현한다. FVCoFreeDReceiver 클래스는 FVCoroutineBase 클래스를 상속하고 Run() 함수를 override 한다.

![](https://github.com/Devcoder-IndieWorks/FreeDReceivePluginSample/blob/master/Images/Use_Coroutine.png)

Run() 함수를 override한 내용은 다음 그림과 같다.

![](https://github.com/Devcoder-IndieWorks/FreeDReceivePluginSample/blob/master/Images/Use_Coroutine_Implementation.png)

(1)번 항목이 매크로를 이용하여  Co Routine처럼 작동할 부분에 대한 범위를 설정해 준다. (2-1)번은 Run() 함수 시작시 Delay를 주기 위해 VCoYieldReturnWait() 매크로를 사용하여 그 위치에서 지정된 시간만큼 대기 처리를 한다. (2-2)번은 while 문안에서 Interval 간격만큼 대기 처리를 한다.

(3-1)번의 Action 함수는 매프레임 호출된다. (3-2)번 DelayAction 함수는 밑에 선언된 VCoYieldReturnWait에 설정된 Interval 단위로 실행된다.

Run() 함수의 실행 흐름은 (3-1)은 매프레임 호출되는 Run() 함수에 따라 호출되고, (1)번 항목에서 VCoroutineReEnter에서 switch문에서 평가한 코드 라인으로 건너띈다. Run() 함수가 최초 처음 호출이면 VCoroutineEntry: 매크로가 선언된 위치에서부터 실행된다. 그리고 (2-1)번 항목에서 Delay 만큼 대기를 하기 위해 (2-1)번 위치에서 goto문이 호출되어 VCoroutineEntry로 코드 호출이 일어나고(goto BailOutOfCoroutine) 그 위치에서 Run() 함수에서 리턴한다. Delay 만큼 시간이 지나게 되면 그 이후 코드가 실행되고, (2-2)번 항목인 VCoYieldReturnWait에서 Interval만큼 대기를 하기 위해 (2-2)번 위치에서 goto문이 호출되어 VCoroutineEntry로 코드 호출이 일어난다.  Run() 함수 재진입시 (1)번 항목인 VCoroutineReEnter에서 switch문으로 코드 라인을 평가하여 (2-2)번 항목으로 건너띄어 다시 Interval 만큼 시간이 지났는지 확인 후 무한 루프인 while문 내 구현된 내용들을 처리한다.

무한 루프인 while문이지만, 코드는 매프레임 리턴하게 된다. 그리고 (2-1)번과 바로 밑에 구현된 내용은 한번만 호출되고 이후에는 호출되지 않는다.

