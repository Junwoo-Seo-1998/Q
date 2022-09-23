is_dead=false
time=0.3
texture="PlayerBullet1"
particleEmitterName = "BulletFragments"

function OnInit()
    trans_comp=CurrentScene:GetTransformComponent(MyID)
	playerID=Utils.GetGameStateManager():GetPlayerID()
	is_flipped=CurrentScene:GetOffsetComponent(playerID):GetIsFlipped()
	offset_comp=CurrentScene:GetOffsetComponent(MyID)
	
	if(is_flipped) then
		 offset_comp:SetFlipped(true)
         trans_comp:SetScale(-3,3)
    else
		 offset_comp:SetFlipped(false)
         trans_comp:SetScale(3,3)
    end
	
    texture_comp=CurrentScene:GetTextureComponent(MyID)
    texture_comp:SetTexture(Utils.GetHashID(texture))
    print(EBulletType.DIRECTDAMAGE)
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
    position = CurrentScene:GetTransformComponent(MyID):GetPosition()

    myVelocity = CurrentScene:GetVelocityComponent(MyID):GetVelocity()
    collidorVelocity = Vector2.new(0.0, 0.0)

	if(TypeID.MapEntity==typeID)then
		staticrect_comp = CurrentScene:GetStaticRectComponent(entt_id)
		if(staticrect_comp:IsLadder()==false)then
			is_dead=true
	
			minYVel = -100.0
			maxYVel = 100.0
	
			minXVel = -50.0
			maxXVel = -200.0
			if(myVelocity.x < 0) then
				minXVel = -minXVel;
				maxXVel = -maxXVel;
			end
	
			Utils.EmitParticle("BulletFragments", 20, position, collidorVelocity, Vector2.new(minXVel, minYVel), Vector2.new(maxXVel, maxYVel),
					Vector2.new(1., 2.), Vector2.new(0.3, 1.0), 0.0)
			end
			print("mapcolidedead")
		end

    if(TypeID.EnemyEntity==typeID) then
		if(CurrentScene:GetHealthComponent(entt_id):IsDead())then
			return
		end
		
        is_dead=true

        collidorVelocity = CurrentScene:GetVelocityComponent(entt_id):GetVelocity()
        collidorVelocity.y = 0;
        minYVel = -100.0
		maxYVel = 100.0
	
		minXVel = -50.0
		maxXVel = -200.0
		if(myVelocity.x < 0) then
			minXVel = -minXVel;
			maxXVel = -maxXVel;
		end
	
		Utils.EmitParticle("BulletFragments", 20, position, collidorVelocity, Vector2.new(minXVel, minYVel), Vector2.new(maxXVel, maxYVel),
				Vector2.new(1., 2.), Vector2.new(0.3, 1.0), 0.0)

        if (not CurrentScene:HasComponent(TypeID.DamageComponent,entt_id)) then
            CurrentScene:AddDamageComponent(typeID,entt_id,60,0.0,EBulletType.DIRECTDAMAGE)
		else
			CurrentScene:GetDamageComponent(entt_id):AddDamage(60,0.0,EBulletType.DIRECTDAMAGE)
        end
    end
end