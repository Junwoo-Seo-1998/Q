function OnInit()
    ButtonType = "Slider"
    Base = ""
    texture=CurrentScene:GetTextureComponent(MyID)
    CurrentTextureID = texture:GetTextureID()
    DefaultTextureID = Utils.GetHashID("MenuBox")
    FocusedTextureID = Utils.GetHashID("MenuBoxInteract")

    SliderTextureID = Utils.GetHashID("Slide")
    SliderLayerTextureID = Utils.GetHashID("SliderLayer")

    BoxSize = CurrentScene:GetWidthHeightComponent(MyID):GetWidthHeight()
    BoxPos = CurrentScene:GetTransformComponent(MyID):GetPosition()

    CurLevelAsFloat = 0.0

    SliderSize = Vector2.new(BoxSize.y * 0.3, BoxSize.y * 0.3)
    SliderLayerSize = Vector2.new(BoxSize.x * 1.2, BoxSize.y * 0.05)


    SliderLayerPos = Vector2.new(BoxPos.x - (SliderLayerSize.x - BoxSize.x)*0.5, 
        BoxPos.y + BoxSize.y * 1.2)

    SliderPos = Vector2.new(SliderLayerPos.x + SliderLayerSize.x * CurLevelAsFloat - SliderSize.x * 0.5 
        , SliderLayerPos.y + SliderLayerSize.y * 0.5 -SliderSize.y * 0.5)

    Levels = 10

    MoveDistance = 1.0 / Levels



    IsFocused = false
end

function OnUpdate(dt)
    SliderPos = Vector2.new(SliderLayerPos.x + SliderLayerSize.x * CurLevelAsFloat - SliderSize.x * 0.5 
        , SliderLayerPos.y + SliderLayerSize.y * 0.5 -SliderSize.y * 0.5)


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