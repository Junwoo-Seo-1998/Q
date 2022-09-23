is_dead=false
time=1
animation="PoisonAnim"

function OnInit()
    ani_comp=CurrentScene:GetAnimationComponent(MyID)
    ani_comp:SetAnimation(Utils.GetHashID(animation),ZDepth.new(0),EFrameOfReference.UNCHANGED,true)
    trans_comp=CurrentScene:GetTransformComponent(MyID)
    trans_comp:SetScale(2,2)
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
		end
    end
    if(TypeID.EnemyEntity==typeID) then
        is_dead=true
        if (not CurrentScene:HasComponent(TypeID.DamageComponent,entt_id)) then
            CurrentScene:AddDamageComponent(typeID,entt_id,80,4,EBulletType.POISONDAMAGE)
        end
    end
end