액션게임을 위한 언리얼 기반 플러그인 모음
Unreal plugins for CombatGame

구성
CfCommon : 공통 (유틸리티 클래스, 로그 등)
CfSkillSystem : 스킬을 위한 시스템
 * Components
   - UCfActionComponent : 스킬 관리 하는 컴퍼넌트
   - UCfHitBoxComponent : 히트 박스 컴퍼넌트, 여러개 붙일 수 있다.
   - UCfSkillInputComponent : 스킬 입력을 관리하는 컴퍼넌트. 로컬 플레이어만 붙는다.
   - UCfStatComponent : Stat 관리
 * Notifies
   - UCfAnimNotify_SkillPreInput : 선입력 시작
   - UCfAnimNotify_SkillCanCancel : 캔슬 구간 시작
   - UCfAnimNotify_SkillEnd : 스킬 종료, 아무키로나 캔슬 가능하다.
 * UCfActionBase : 몽타주 관리하는 액션 객체. 새로 시작할때마다 NewObject<>() 로 생성한다.
   - UCfActionSkill : 평타 및 스킬 관리
   - UCfActionJump : 몽타주는 없지만 점프시 스킬로 친다.
   - UCfActionHit : 히트시 생성된다.
   - UCfActionRecovery : 넘어지는 히트가 끝났을 경우 이 액션이 예약되어 플레이된다.
CfSkillSystemEditor : Notify 클릭했을때 Draw 한다.
