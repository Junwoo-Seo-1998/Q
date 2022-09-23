is_dead=false
time=3
texture="PoisonBomb"


already_create_effect=false
function OnInit()
    texture_comp=CurrentScene:GetTextureComponent(MyID)
    texture_comp:SetTexture(Utils.GetHashID(texture))
    trans_comp=CurrentScene:GetTransformComponent(MyID)
    trans_comp:SetScale(1.2,1.2)
	--CurrentScene:AddTagComponent(TypeID.ProjectileEntity,MyID,"Bomb") don't break tiles
	--print("BombAddTag")
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
		staticrect_comp = CurrentScene:GetStaticRectComponent(entt_id)
		if(staticrect_comp:IsLadder()==false)then
			is_dead=true
			transform = CurrentScene:GetTransformComponent(MyID)
			pos= transform:GetPosition()

		if(already_create_effect==false) then
		    Sound_Component = Utils.GetSoundComponent()
			Sound_Component:PlaySoundEffect("bomb-explosion", false)
            Create.AnimationWithScript(pos,Vector2.new(1,1),Vector2.new(0.3,0.3),"resource/lua/Simple/SimplePoisonAnimation.lua")
            already_create_effect=true
        end
		for i = 0, 12, 1 do
			velo=Vector2.new(1000*math.cos(i*0.323599),1000*math.sin(i*0.323599))
			Create.ProjectileAnimationWithGravity(pos,velo,Vector2.new(0.3,0.3),"resource/lua/Projectile/PoisonFrag.lua")
		end
		
		
        Utils.ShakeMainCamera(0.2)
		end
    end
end