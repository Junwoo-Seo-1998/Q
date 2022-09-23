is_dead=false
time=1
animation="ExplodeVerticalAnim"

function OnInit()
    trans_comp=CurrentScene:GetTransformComponent(MyID)
   
    offset_comp=CurrentScene:GetOffsetComponent(MyID)
	
	playerID=Utils.GetGameStateManager():GetPlayerID()
	is_flipped=CurrentScene:GetOffsetComponent(playerID):GetIsFlipped()
	

	offset_comp:SetFlipped(false)
	trans_comp:SetScale(3,3)

    offset_comp:SetHotSpotPosition(Vector2.new(100,60))
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
