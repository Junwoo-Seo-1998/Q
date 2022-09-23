is_dead=false
time=3
time_acc=0
boost_timer=0.5
texture="Missile"
init_y=0
function OnInit()
    trans_comp=CurrentScene:GetTransformComponent(MyID)
    trans_comp:SetScale(3,3)

    velo = CurrentScene:GetVelocityComponent(MyID):GetVelocity()
    velo.x=velo.x;
    if(velo.x<0.0) then
        texture=texture.."-Left"
    end
    texture_comp=CurrentScene:GetTextureComponent(MyID)
    texture_comp:SetTexture(Utils.GetHashID(texture))
    
    

    

    CurrentScene:GetVelocityComponent(MyID):SetVelocity(velo)
    position = CurrentScene:GetTransformComponent(MyID):GetPosition()
    init_y=position.y
end

function OnUpdate(dt)
    time=time-dt
    time_acc=time_acc+dt
    boost_timer=boost_timer-dt
    position = CurrentScene:GetTransformComponent(MyID):GetPosition()
    position.y=init_y+math.sin(time_acc*30)*15
    CurrentScene:GetTransformComponent(MyID):SetPosition(position)
    if(boost_timer<=0) then
        
        velo = CurrentScene:GetVelocityComponent(MyID):GetVelocity()
        velo.x=velo.x*1.05
        CurrentScene:GetVelocityComponent(MyID):SetVelocity(velo)
    end 

    if(time<=0) then
        is_dead=true
    end
    if(velo.x<0.0) then
        position.y=position.y+5
        Create.TextureWithScript(position,Vector2.new(0,0),Vector2.new(1,1),"resource/lua/Simple/SimpleTexture.lua")
    else
        Create.TextureWithScript(position,Vector2.new(0,0),Vector2.new(1,1),"resource/lua/Simple/SimpleTexture.lua")
    end
end

function OnCleanUp()

end

function OnCollideWith(typeID,entt_id)
    --print("hit!",MyID,typeID, entt_id)
    position = CurrentScene:GetTransformComponent(MyID):GetPosition()

    myVelocity = CurrentScene:GetVelocityComponent(MyID):GetVelocity()
    collidorVelocity = Vector2.new(0.0, 0.0)

	if(TypeID.MapEntity==typeID)then
		staticrect_comp = CurrentScene:GetStaticRectComponent(entt_id)
		if(staticrect_comp:IsLadder()==false)then
			is_dead=true
            
		end
			
	end

    if(TypeID.EnemyEntity==typeID) then
        is_dead=true
        if (not CurrentScene:HasComponent(TypeID.DamageComponent,entt_id)) then
            CurrentScene:AddDamageComponent(typeID,entt_id,300,0.0,EBulletType.DIRECTDAMAGE)
		else
			CurrentScene:GetDamageComponent(entt_id):AddDamage(300,0.0,EBulletType.DIRECTDAMAGE)
		end
    end
    if(is_dead) then
    
        pos= CurrentScene:GetTransformComponent(MyID):GetPosition()
        for i = 0, 12, 1 do
            velo=Vector2.new(1000*math.cos(i*0.323599),1000*math.sin(i*0.323599))
            Create.ProjectileAnimationWithGravity(pos,velo,Vector2.new(0.3,0.3),"resource/lua/Projectile/BasicFrag.lua")
        end
        Create.AnimationWithScript(position,Vector2.new(0,0),Vector2.new(1,1),"resource/lua/Simple/SimpleAnimation.lua")
        Sound_Component = Utils.GetSoundComponent()
		Sound_Component:PlaySoundEffect("bomb-explosion", false)
    end
end