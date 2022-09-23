
function OnInit()
    CamMoveSpeed=1000
end


function OnUpdate(dt)
    transform=CurrentScene:GetTransformComponent(MyID)
    if(Input.IsKeyDown(Key.W)) then
        pos=transform:GetPosition()
        pos.y=pos.y+CamMoveSpeed*dt
        transform:SetPosition(pos)
    end 

    if(Input.IsKeyDown(Key.A)) then
        pos=transform:GetPosition()
        pos.x=pos.x-CamMoveSpeed*dt
        transform:SetPosition(pos)
    end 

    if(Input.IsKeyDown(Key.S)) then
        pos=transform:GetPosition()
        pos.y=pos.y-CamMoveSpeed*dt
        transform:SetPosition(pos)
    end 

    if(Input.IsKeyDown(Key.D)) then
        pos=transform:GetPosition()
        pos.x=pos.x+CamMoveSpeed*dt
        transform:SetPosition(pos)
    end 

end

function OnCleanUp()

end