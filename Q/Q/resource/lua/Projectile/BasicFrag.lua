is_dead=false
time=1
animation="ExplodePurpleAnim"

function OnInit()
    ani_comp=CurrentScene:GetAnimationComponent(MyID)
    ani_comp:SetAnimation(Utils.GetHashID(animation),ZDepth.new(0),EFrameOfReference.UNCHANGED,true)
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
            CurrentScene:AddDamageComponent(typeID,entt_id,300,0.0,EBulletType.DIRECTDAMAGE)
        end
    end
end