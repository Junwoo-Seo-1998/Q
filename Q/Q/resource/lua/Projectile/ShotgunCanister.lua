is_dead=false
time=2
texture="Shotguncanister"

function OnInit()
    trans_comp=CurrentScene:GetTransformComponent(MyID)
	playerID=Utils.GetGameStateManager():GetPlayerID()
	
    trans_comp:SetScale(3,3)
    texture_comp=CurrentScene:GetTextureComponent(MyID)
    texture_comp:SetTexture(Utils.GetHashID(texture))
    vel_comp=CurrentScene:GetVelocityComponent(MyID)
    velo=vel_comp:SetAngularVelocity(2)
end

function OnUpdate(dt)
    time=time-dt
    if(time<=0) then
        is_dead=true
    end
end

function OnCleanUp()

end

function OnCollideWith(typeID,entt_id)
    --print("hit!",MyID,typeID, entt_id)
    if(TypeID.MapEntity==typeID) then
        vel_comp=CurrentScene:GetVelocityComponent(MyID)
        velo=vel_comp:GetVelocity()
        x,y=rand_v_dir()
        velo.x=-(velo.x+x*20)*0.8
        velo.y=-(velo.y+y*10)*0.8
        vel_comp:SetVelocity(velo)
    end
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