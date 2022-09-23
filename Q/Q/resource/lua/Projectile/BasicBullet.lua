is_dead=false
time=1
texture="PlayerBullet1"
particleEmitterName = "BulletFragments"

function OnInit()
    trans_comp=CurrentScene:GetTransformComponent(MyID)
    trans_comp:SetScale(3,3)
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
    colliderVelocity = Vector2.new(0.0, 0.0)


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
	
			Utils.EmitParticle("BulletFragments", 20, position, colliderVelocity, Vector2.new(minXVel, minYVel), Vector2.new(maxXVel, maxYVel),
					Vector2.new(1., 2.), Vector2.new(0.3, 1.0), 0.0)
			end
			print("mapcolidedead")
		end

    if(TypeID.EnemyEntity==typeID) then
		if(CurrentScene:GetHealthComponent(entt_id):IsDead())then
			return
		end
		
        is_dead=true

        colliderVelocity = CurrentScene:GetVelocityComponent(entt_id):GetVelocity()
        colliderVelocity.y = 0;
        minYVel = -100.0
		maxYVel = 100.0
	
		minXVel = -50.0
		maxXVel = -200.0
		if(myVelocity.x < 0) then
			minXVel = -minXVel;
			maxXVel = -maxXVel;
		end
	
		Utils.EmitParticle("BulletFragments", 20, position, colliderVelocity, Vector2.new(minXVel, minYVel), Vector2.new(maxXVel, maxYVel),
				Vector2.new(1., 2.), Vector2.new(0.3, 1.0), 0.0)

        if (not CurrentScene:HasComponent(TypeID.DamageComponent,entt_id)) then
            CurrentScene:AddDamageComponent(typeID,entt_id,100,0.0,EBulletType.DIRECTDAMAGE)
		else
			CurrentScene:GetDamageComponent(entt_id):AddDamage(100,0.0,EBulletType.DIRECTDAMAGE)
		end
    end
end