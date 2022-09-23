FollowFlip = true
TimerID = -1
MyTimer = nil
animation = "ExplodeVerticalAnim"

function OnInit()
    DoDestroy = false

    mySize = CurrentScene:GetWidthHeightComponent(MyID):GetWidthHeight()

    myX = Utils.GetFloatRandomValue(0.0, mySize.x)
    myY = Utils.GetFloatRandomValue(0.0, mySize.y)

    rot = Utils.GetFloatRandomValue(0.0, 360.0)
    -- rot = 0.0
    entData = Create.FollowAnimation(Utils.GetHashID(animation), MyID, Vector2.new(myX * 0.5, myY * 0.5), Vector2.new(1.0, 1.0), rot)
 
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