is_dead=false
time=1
texture="cross"

function OnInit()
    trans_comp=CurrentScene:GetTransformComponent(MyID)
    trans_comp:SetScale(3,3)
    texture_comp=CurrentScene:GetTextureComponent(MyID)
    texture_comp:SetTexture(Utils.GetHashID(texture))
end

function OnUpdate(dt)
    time=time-dt
    if(time<=0) then
        is_dead=true
    end
end

function OnCleanUp()

end
