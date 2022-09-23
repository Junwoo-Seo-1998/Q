----can change------
WeaponType="Ranged"
Max_ammo=10
reload_time=4
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
    print("Gun!")
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
        print("reloading...")
        Sound_Component:PlaySoundEffect("gun-reload",false)
        transform = CurrentScene:GetTransformComponent(playerID)
        pos= transform:GetPosition()
        velo_mag=Vector2.new(-10,10)
        velo_mag.x=velo_mag.x*10
        is_flipped=CurrentScene:GetOffsetComponent(playerID):GetIsFlipped()
        if(is_flipped) then
            velo_mag.x=-velo_mag.x*3
        end
        velo_mag.y=velo_mag.y+500
        Create.ProjectileWithGravity(pos,velo_mag,Vector2.new(2,2),"resource/lua/Projectile/Magazine.lua")
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

    --canister
    x,y=rand_v_dir ()
    velo_cani=Vector2.new(x,y)
    velo_cani.y=100
    pos.y=pos.y+45
    Create.ProjectileWithGravity(pos,velo_cani,Vector2.new(2,2),"resource/lua/Projectile/BasicCanister.lua")

    if(is_flipped) then
        pos.x=pos.x-10
        velo=Vector2.new(-1000,0)
    else
        pos.x=pos.x+10
    end

    velo.y = Utils.GetFloatRandomValue(-50.0, 50.0);

    Utils.ShakeMainCamera(0.2, 0.3)
    Create.Projectile(pos,velo,Vector2.new(0.3,0.3),"resource/lua/Projectile/BasicBullet.lua")


    TestTraceBack.LogWeaponUseMessage("BasicGun")
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