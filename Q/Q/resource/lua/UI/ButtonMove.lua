function OnInit()
    ButtonType = "Select"
    texture=CurrentScene:GetTextureComponent(MyID)
    CurrentTextureID = texture:GetTextureID()
    DefaultTextureID = Utils.GetHashID("MenuBox")
    FocusedTextureID = Utils.GetHashID("MenuBoxInteract")
    IsFocused = false
end

function OnUpdate(dt)

    if(IsFocused) then
        if(CurrentTextureID ~= FocusedTextureID) then
            CurrentTextureID = FocusedTextureID;
            texture:SetTexture(CurrentTextureID)
            Sound_Component = Utils.GetSoundComponent()
            Sound_Component:PlaySoundEffect("button-interaction", false)
        end
    else
        if(CurrentTextureID ~= DefaultTextureID) then
            CurrentTextureID = DefaultTextureID;
            texture:SetTexture(CurrentTextureID)
        end
    end
end

function OnCleanUp()

end