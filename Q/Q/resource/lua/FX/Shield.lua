FollowFlip = true
TimerID = -1
MyTimer = nil
animation1 = "ShieldAnim"
animation2 = "ShieldFadeOutAnim"

scalar = Vector2.new(4.0, 4.0)

Idx = 0;
function OnInit()
    DoDestroy = false
    followEntSize = CurrentScene:GetWidthHeightComponent(MyID):GetWidthHeight()
    folloowEntOffset = CurrentScene:GetOffsetComponent(MyID):GetHotSpotPosition()


    myAnimSize = AssetManager.GetAnimationSize(Utils.GetHashID(animation1))
    myAnimSize = Vector2.new(myAnimSize.x * scalar.x, myAnimSize.y * scalar.y)
    -- myX = Utils.GetFloatRandomValue(0.0, followEntSize.x)
    -- myY = Utils.GetFloatRandomValue(0.0, followEntSize.y)
    myOffset = Vector2.new((followEntSize.x - myAnimSize.x) * -0.5 - folloowEntOffset.x * 0.5, (followEntSize.y - myAnimSize.y)* -0.5 - folloowEntOffset.y * 0.5)
    -- myOffset = Vector2.new(followEntSize.x * -0.5, -10.0)

    rot = 0.0

    entData = Create.FollowAnimation(Utils.GetHashID(animation1), MyID, myOffset, scalar, rot)

    BaseEntTypeID = entData[1]
    BaseEntID = entData[2]

    AnimationComp = CurrentScene:GetAnimationComponent(BaseEntID)

    TimerID = TimerManager.IssueTimer(3.0)

    MyTimer = TimerManager.GetTimer(TimerID)
    Sound_Component = Utils.GetSoundComponent()
    Sound_Component:PlaySoundEffect("shield_hit",false)
end

function OnUpdate(dt)
    MyTimer = TimerManager.GetTimer(TimerID)
    if(MyTimer:IsOnTimeOut()) then
        if(Idx == 0) then
            TimerManager.YieldTimer(TimerID)
            TimerID = -1
            
            AnimationComp:ChangeAnimation(Utils.GetHashID(animation2))
            -- AnimationComp:SetAnimation(Utils.GetHashID(animation2))

            -- SizeComponent = CurrentScene:GetWidthHeightComponent(BaseEntID)
            -- OffsetComponent = CurrentScene:GetOffsetComponent(BaseEntID)

            -- myAnimSize = AssetManager.GetAnimationSize(Utils.GetHashID(animation2))
            -- myAnimSize = Vector2.new(myAnimSize.x * scalar.x, myAnimSize.y * scalar.y)
            -- myOffset = Vector2.new((followEntSize.x - myAnimSize.x) * -0.5, (followEntSize.y - myAnimSize.y)* -0.5)

            -- SizeComponent:SetWidthHeight(myAnimSize)
            -- OffsetComponent:SetHotSpotPosition(myOffset)
            -- Trans
            TotalAnimTime = AnimationComp:GetLength()
            TimerID = TimerManager.IssueTimer(TotalAnimTime)
            MyTimer = nil
            MyTimer = TimerManager.GetTimer(TimerID)
            MyTimer:ResetAndResume()
            Idx = 1
            return
        end
        if(Idx == 1) then
            TimerManager.YieldTimer(TimerID)
            TimerID = -1
            MyTimer = nil
            Idx = 3
            DoDestroy = true
        end
    end
end

function OnCleanUp()
    if(TimerID ~= -1)  then
        TimerManager.YieldTimer(TimerID)
        TimerID = -1
        MyTimer = nil
    end
end