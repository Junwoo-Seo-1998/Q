function OnInit()
    ButtonType = "CheckBox"
    CheckBoxType = "FullScreen"

    IsFocused = false
    IsChecked = false
    WasChecked = false
    UnCheckedTexture = Utils.GetHashID("UnChecked")
    CheckedTexture = Utils.GetHashID("Checked")

    texture=CurrentScene:GetTextureComponent(MyID)
    CurrentTextureID = texture:GetTextureID()
    DefaultTextureID = Utils.GetHashID("MenuBox")
    FocusedTextureID = Utils.GetHashID("MenuBoxInteract")


    BoxSize = CurrentScene:GetWidthHeightComponent(MyID):GetWidthHeight()
    BoxPos = CurrentScene:GetTransformComponent(MyID):GetPosition()

    checkBoxSize = Vector2.new(BoxSize.y * 0.7, BoxSize.y * 0.7)
    checkBoxPos = Vector2.new(BoxPos.x + (BoxSize.x - checkBoxSize.x) * 0.5, BoxPos.y + BoxSize.y * 1.1)

    CurrentCheckBoxTextureID = -1

    if(IsChecked) then
        CurrentCheckBoxTextureID = CheckedTexture
    else
        CurrentCheckBoxTextureID = UnCheckedTexture
    end
end

function OnUpdate(dt)
    if(IsChecked)then
        CurrentCheckBoxTextureID = CheckedTexture
    else
        CurrentCheckBoxTextureID = UnCheckedTexture
    end
    
    if(IsFocused) then
        if(WasChecked ~= IsChecked)then

        end
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
    WasChecked = IsChecked
end

function OnCleanUp()

end