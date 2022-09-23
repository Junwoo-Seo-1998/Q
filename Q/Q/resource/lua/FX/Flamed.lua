FollowFlip = false
TimerID = -1
MyTimer = nil
animation = "ExplodeVerticalFlameAnim"

function OnInit()
    DoDestroy = false
    scalar = 1.0

    rot = Utils.GetFloatRandomValue(0.0, 360.0)
    followEntSize = CurrentScene:GetWidthHeightComponent(MyID):GetWidthHeight()
    folloowEntOffset = CurrentScene:GetOffsetComponent(MyID):GetHotSpotPosition()



    myAnimSize = AssetManager.GetAnimationSize(Utils.GetHashID(animation))

    myAnimSize = Vector2.new(myAnimSize.x * scalar, myAnimSize.y * scalar)

    scalar = followEntSize.y / myAnimSize.y * 1.5;

    myOffset = Vector2.new((followEntSize.x - myAnimSize.x) * 0.5, (followEntSize.y - myAnimSize.y)* 0.5)



    entData = Create.FollowAnimation(Utils.GetHashID(animation), MyID, myOffset, Vector2.new(scalar, scalar), rot)
 
    BaseEntTypeID = entData[1]
    BaseEntID = entData[2]

    AnimationComp = CurrentScene:GetAnimationComponent(BaseEntID)
    TotalAnimTime = AnimationComp:GetLength()
    TimerID = TimerManager.IssueTimer(TotalAnimTime)

    MyTimer = TimerManager.GetTimer(TimerID)
end

function OnUpdate(dt)
    -- MyTimer = TimerManager.GetTimer(TimerID)
    if(MyTimer:IsOnTimeOut()) then
        TimerManager.YieldTimer(TimerID)
        TimerID = -1
        DoDestroy = true
    end
end

function OnCleanUp()
    if(TimerID ~= -1) then
        TimerManager.YieldTimer(TimerID)
        TimerID = -1
        MyTimer = nil
    end
end