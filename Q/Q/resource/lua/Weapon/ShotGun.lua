----can change------
WeaponType="Ranged"
Max_ammo=2
reload_time=2
----do not change---
playerID=0
IsAttacking=false
IsReloading=false
Current_ammo=0
Reload_Timer_ID=-1
Reload_Timer=nil
Sound_Component=nil
Player_State_Comp=nil
--------------------

function OnInit()
    print("ShotGun!")
    Sound_Component = Utils.GetSoundComponent()
    playerID=Utils.GetGameStateManager():GetPlayerID()
    Player_State_Comp=CurrentScene:GetObjectStateComponent(playerID)
    Reload_Timer_ID = TimerManager.IssueTimer(reload_time)
    Reload_Timer = TimerManager.GetTimer(Reload_Timer_ID)
    Current_ammo=Max_ammo
end

function OnUpdate(dt)
    if(Input.IsKeyPressed(Key.X)) then
        Attack()
    else 
        IsAttacking=false
    end
    

    --//check we should reload 
    if(Current_ammo==0) then
        Reload(dt)
    end

end

function OnCleanUp()
    TimerManager.YieldTimer(Reload_Timer_ID)
    Reload_Timer_ID = -1
end

function Reload()
    if(IsReloading==false) then
        IsReloading=true
        Reload_Timer:ResetAndResume()

        transform = CurrentScene:GetTransformComponent(playerID)
        pos= transform:GetPosition()

        velo=Vector2.new(1000,0)
 
        --canister
        is_flipped=CurrentScene:GetOffsetComponent(playerID):GetIsFlipped()
        x,y=rand_v_dir ()
        velo_cani=Vector2.new(x,y)
        velo_cani.y=100
        pos.y=pos.y+45
        Create.ProjectileWithGravity(pos,velo_cani,Vector2.new(2,2),"resource/lua/Projectile/ShotgunCanister.lua")
        pos.x=pos.x+10
        pos.y=pos.y+3
        if(is_flipped) then
            velo_cani.x=velo_cani.x+10
        else
            velo_cani.x=velo_cani.x-10
        end 
        velo_cani.y=velo_cani.y+10
        Create.ProjectileWithGravity(pos,velo_cani,Vector2.new(2,2),"resource/lua/Projectile/ShotgunCanister.lua")
        Sound_Component:PlaySoundEffect("shotgun-reload",false)
        print("reloading...")
    end

    if(Reload_Timer:IsOnTimeOut()) then
        IsReloading=false
        Current_ammo=Max_ammo
        print("reloading done!")
        return
    end
end


function Attack()

    ---------------------------------------
    --condition when cannot attack---------
    ---------------------------------------

    --//check is on climbing status or dead
    player_state=Player_State_Comp:GetPlayerMovingState()
    
    if(player_state==MovingState.CLIMBING or player_state==MovingState.CLIMBINGUP
        or player_state==MovingState.CLIMBINGDOWN or player_state==MovingState.ISDEAD) then
            return;
    end
    --//check is reloading now
    if(IsReloading==true) then
        Sound_Component:PlaySoundEffect("out-of-ammo",false)
        return
    end

    ---------------------------------------
    IsAttacking=true --// for player animation
    Current_ammo=Current_ammo-1 --// need to decrease bullet num
    ---------------------------------------

    Sound_Component:PlaySoundEffect("gun-shot",false)
    transform = CurrentScene:GetTransformComponent(playerID)
    pos= transform:GetPosition()
    is_flipped=CurrentScene:GetOffsetComponent(playerID):GetIsFlipped()
    
    velo=Vector2.new(1000,0)
 
    pos.y=pos.y+45
    --Create.ProjectileWithGravity(pos,velo_cani,Vector2.new(2,2),"resource/lua/Projectile/ShotgunCanister.lua")

    if(is_flipped) then
        pos.x=pos.x-10
        velo=Vector2.new(-1000,0)
    else
        pos.x=pos.x+10
    end


	
    Utils.ShakeMainCamera(0.2, 0.3)

	for i = 5, -5, -1 do
	if(is_flipped) then
		velo1=Vector2.new(-1000,40*i)
		velo1.x=Utils.GetFloatRandomValue(-1000,-700)
		Create.Projectile(pos,velo1,Vector2.new(0.3,0.3),"resource/lua/Projectile/ShotgunBullet.lua")
	else
		velo1=Vector2.new(1000,40*i)
		velo1.x=Utils.GetFloatRandomValue(700,1000)
		Create.Projectile(pos,velo1,Vector2.new(0.3,0.3),"resource/lua/Projectile/ShotgunBullet.lua")
	end
	end

    TestTraceBack.LogWeaponUseMessage("ShotGun")
end

function rand_v_dir ()

    vx = math.random()
    vy = math.random()
 
    norm = math.sqrt(vx * vx + vy * vy)
    vx = vx / norm
    vy = vy / norm
 
    if math.random(0, 1) == 0 then
       vx = -vx
    end
 
    if math.random(0, 1) == 0 then
       vy = -vy
    end
 
    return vx, vy
 end