playerID=0
TimerID = -1
MyTimer = nil
ReloadingTime = 10.0
function OnInit()
    playerID=Utils.GetGameStateManager():GetPlayerID()
    TimerID = TimerManager.IssueTimer(ReloadingTime)
    MyTimer = TimerManager.GetTimer(TimerID)
    MyTimer:SetTimeOut()
end

function OnUpdate(dt)
    MyTimer = TimerManager.GetTimer(TimerID)
        if(Input.IsKeyPressed(Key.S)) then
            UseItem()
        end
end

function OnCleanUp()
    TimerManager.YieldTimer(TimerID)
    TimerID = -1
end

function UseItem()
    --do something
    --MyTimer:ResetAndResume()
    inventory=CurrentScene:GetNewPlayerInventorySlots(playerID)
    inventory:ItemUsed()


    itemComp = CurrentScene:GetNewItemComponent(playerID)
    itemComp:NotifyItemUIUsed()
    print("bomb!")
    transform = CurrentScene:GetTransformComponent(playerID)
    is_flipped=CurrentScene:GetOffsetComponent(playerID):GetIsFlipped()
    pos= transform:GetPosition()
    velo=Vector2.new(500,0)
    if(is_flipped) then
        pos.x=pos.x-10
        velo=Vector2.new(-500,0)
    else
        pos.x=pos.x+10
    end

    player_state_comp=CurrentScene:GetObjectStateComponent(playerID)
    player_state=player_state_comp:GetPlayerMovingState()
    
    if(player_state==MovingState.CLIMBING or player_state==MovingState.CLIMBINGUP
    or player_state==MovingState.CLIMBINGDOWN) then
        velo.x=0
        velo.y=1000
    end

    Create.ProjectileWithGravity(pos,velo,Vector2.new(10,10),"resource/lua/Projectile/BasicBomb.lua")
    TestTraceBack.LogItemUseMessage("Bomb")

end