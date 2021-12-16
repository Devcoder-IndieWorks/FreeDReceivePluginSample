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

