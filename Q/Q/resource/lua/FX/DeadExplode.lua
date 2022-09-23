FollowFlip = false
TimerID = -1
MyTimer = nil
animation = "ExplodeRoundGreenAnim"

function OnInit()
    -- for FXsystem
    DoDestroy = false
    scalar = 1.0
    -- mySize = CurrentScene:GetWidthHeightComponent(MyID):GetWidthHeight()

    -- myX = Utils.GetFloatRandomValue(-mySize.x * 0.5, mySize.x * 0.5)
    -- myY = Utils.GetFloatRandomValue(-mySize.y * 0.5, mySize.y * 0.5)

    -- myOffset = Vector2.new(mySize.x * -0.5 + myX, mySize.y * -0.5 + myY)

    -- rot = Utils.GetFloatRandomValue(0.0, 360.0)

    size = Utils.GetFloatRandomValue(1.0, 5.0)


    rot = Utils.GetFloatRandomValue(0.0, 360.0)
    followEntSize = CurrentScene:GetWidthHeightComponent(MyID):GetWidthHeight()
    folloowEntOffset = CurrentScene:GetOffsetComponent(MyID):GetHotSpotPosition()

    myX = Utils.GetFloatRandomValue(-followEntSize.x * 0.5, followEntSize.x * 0.5)
    myY = Utils.GetFloatRandomValue(-followEntSize.y * 0.5, followEntSize.y * 0.5)

    animIdx =  Utils.GetIntRandomValue(1, 4)

    finalAnim = animation .. tostring(animIdx)

    myAnimSize = AssetManager.GetAnimationSize(Utils.GetHashID(finalAnim))

    myAnimSize = Vector2.new(myAnimSize.x * scalar, myAnimSize.y * scalar)

    scalar = size;

    myOffset = Vector2.new((followEntSize.x - myAnimSize.x) * 0.5 + myX, (followEntSize.y - myAnimSize.y)* 0.5 + myY)


    entData = Create.FollowAnimation(Utils.GetHashID(finalAnim), MyID, myOffset, Vector2.new(scalar, scalar), rot)

    -- entData = Create.FollowAnimation(Utils.GetHashID(animation), MyID, Vector2.new(myX, myY), Vector2.new(size, size), rot)
    
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