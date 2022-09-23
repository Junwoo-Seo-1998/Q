playerID=0
HP_to_add=50

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
    --MyTimer:ResetAndResume()
		health_comp=CurrentScene:GetHealthComponent(playerID)
		if(health_comp:GetCurHealth() == health_comp:GetMaxHealth()) then
			return
		end
	
	
    inventory=CurrentScene:GetNewPlayerInventorySlots(playerID)
    inventory:ItemUsed()
    MyPosition = CurrentScene:GetTransformComponent(MyID):GetPosition()
    MyVelocity = CurrentScene:GetVelocityComponent(MyID):GetVelocity()
    MyWidthHeight = CurrentScene:GetWidthHeightComponent(MyID):GetWidthHeight()

    EmitPosition = MyPosition + Vector2.new(MyWidthHeight.x * 0.5, MyWidthHeight.x * 0.3)

    --[Utils.EmitParticle("Heal", 100, EmitPosition, MyVelocity, Vector2.new(-20, 120), Vector2.new(20, 200), Vector2.new(0.5, 1.0), Vector2.new(1.0, 2.0), 0.0 )
    



    Sound_Component = Utils.GetSoundComponent()
    Sound_Component:PlaySoundEffect("drink_sound",false)

    health_comp:ChangeCurHealth(HP_to_add)
    Create.TextureWithScript(MyPosition,Vector2.new(0,100),Vector2.new(0.3,0.3),"resource/lua/Simple/SimpleCrossEffect.lua")

    TestTraceBack.LogItemUseMessage("Potion")
end