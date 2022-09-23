is_dead=false
time=1
texture="magazine"

function OnInit()
    trans_comp=CurrentScene:GetTransformComponent(MyID)
    trans_comp:SetScale(2,2)
    texture_comp=CurrentScene:GetTextureComponent(MyID)
    texture_comp:SetTexture(Utils.GetHashID(texture))
    vel_comp=CurrentScene:GetVelocityComponent(MyID)
    vel_comp:SetAngularVelocity(5)
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
        velo.x=-(velo.x+x*20)*0.2
        velo.y=-(velo.y+y*10)*0.2
        vel_comp:SetVelocity(velo)
        vel_comp:SetAngularVelocity(0)
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