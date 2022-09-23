/*
    File Name: Logger.cpp
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Logger.h"
#include <filesystem>

namespace q_engine
{
    Logger::Logger() : show_error(false), show_warning(false), show_notice(false), show_inform(true), show_debug(false)
    {
        char* docdir = getenv("USERPROFILE");
        std::string Savingdir(docdir);
        if (docdir)
        {
            Savingdir += "\\Documents\\Q";
        }
        if (std::filesystem::exists(Savingdir) == false)
        {
            std::filesystem::create_directory(Savingdir);
        }
        std::string Savingpath = Savingdir + "\\Q.log";

        ofs.open(Savingpath, std::ios::out | std::ios::trunc);
    	if(ofs.fail())
    	{
            throw std::logic_error{ "Failed to Open Q.log" };
    	}

    }
    void Logger::AddMessage(const std::string& messege, SeverityLevel lvl)
    {
        if (size_ == max_size_)
        {
            messages_.pop_front();
            messages_.erase(messages_.begin() + static_cast<size_t>(max_size_ * 0.5f), messages_.end());
            size_ = messages_.size();
            ofs.flush();
        }
        Log temp (lvl, messege);
        messages_.emplace_back(temp);
        switch (temp.m_level)
        {
        case SeverityLevel::ERROR:
        {
            if (show_error)
            {
                ofs << temp.GetSeverityLevelString() << "\t" << temp.GetMessegeString() << "\n";
            }
            break;
        }
        case SeverityLevel::WARNING:
        {
            if (show_warning)
            {
                ofs << temp.GetSeverityLevelString() << "\t" << temp.GetMessegeString() << "\n";
            }
            break;
        }
        case SeverityLevel::NOTICE:
        {
            if (show_notice)
            {
                ofs << temp.GetSeverityLevelString() << "\t" << temp.GetMessegeString() << "\n";
            }
            break;
        }
        case SeverityLevel::INFORMATIONAL:
        {
            if (show_inform)
            {
                ofs << temp.GetSeverityLevelString() << "\t" << temp.GetMessegeString() << "\n";
            }
            break;
        }
        case SeverityLevel::DEBUG:
        {
            if (show_debug)
            {
                ofs << temp.GetSeverityLevelString() << "\t" << temp.GetMessegeString() << "\n";
            }
            break;
        }
        }
        ++size_;
    }

    void Logger::ClearMessages()
    {
        messages_.clear();
        size_ = 0;
    }

    void Logger::ImGuiRender(bool& is_open)
    {
        ImGui::Begin("Logger", &is_open, ImGuiWindowFlags_None);

        static bool auto_scroll = true;


        static ImColor error_col{ 1.f, 0.f, 0.f }, warning_col{ 0.5f, 0.2f, 0.5f }, notice_col{ 0.2f, 0.5f, 0.5f },
            inform_col{ 0.f, 0.f, 1.f }, debug_col{ 0.f, 1.f, 0.f };

        if (ImGui::BeginPopup("Options"))
        {
            ImGui::Checkbox("AutoScroll", &auto_scroll);
            ImGui::EndPopup();
        }

        if (ImGui::Button("Options"))
            ImGui::OpenPopup("Options");

        ImGui::SameLine();
        bool clear = ImGui::Button("Clear");

        ImGui::Checkbox("Error", &show_error);  ImGui::SameLine();
        ImGui::Checkbox("Warn", &show_warning);  ImGui::SameLine();
        ImGui::Checkbox("Notice", &show_notice);  ImGui::SameLine();
        ImGui::Checkbox("Inform", &show_inform);  ImGui::SameLine();
        ImGui::Checkbox("Debug", &show_debug);

        ImGui::Separator();
        ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        for (const auto& message : messages_)
        {
            switch(message.m_level)
            {
            case SeverityLevel::ERROR:
                {
                    if(show_error)
                    {
                        ImGui::TextColored(error_col, message.GetSeverityLevelString()); ImGui::SameLine();
                        ImGui::Text(message.GetMessegeString());
                    }
                    break;
                }
            case SeverityLevel::WARNING: 
                {
                    if(show_warning)
                    {
                        ImGui::TextColored(warning_col, message.GetSeverityLevelString()); ImGui::SameLine();
                        ImGui::Text(message.GetMessegeString());
                    }
                    break;
                }
            case SeverityLevel::NOTICE:
                {
                    if (show_notice)
                    {
                        ImGui::TextColored(notice_col, message.GetSeverityLevelString()); ImGui::SameLine();
                        ImGui::Text(message.GetMessegeString());
                    }
                    break;
                }
            	case SeverityLevel::INFORMATIONAL:
                {
                    if (show_inform)
                    {
                        ImGui::TextColored(inform_col, message.GetSeverityLevelString()); ImGui::SameLine();
                        ImGui::Text(message.GetMessegeString());
                    }
                    break;
                }
            case SeverityLevel::DEBUG:
                {
                    if (show_debug)
                    {
                        ImGui::TextColored(debug_col, message.GetSeverityLevelString()); ImGui::SameLine();
                        ImGui::Text(message.GetMessegeString());
                    }
                    break;
                }
            default:
                {
                break;
                }
            }
        }

        if(clear)
        {
            ClearMessages();
        }

        if (auto_scroll && ImGui::GetScrollY() < ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);

        ImGui::EndChild();
        ImGui::End();
    }

    const char* Log::GetSeverityLevelString() const
    {
        switch (m_level)
        {
        case SeverityLevel::ERROR:
            return "[Error_]";
        case SeverityLevel::WARNING:
            return "[Warn__]";
        case SeverityLevel::NOTICE:
            return "[Notice]";
        case SeverityLevel::INFORMATIONAL:
            return "[Inform]";
        case SeverityLevel::DEBUG:
            return "[Debug_]";
        default:
            return "[Unknown]";
        }
    }

    const char* Log::GetMessegeString() const
    {
        return m_message.c_str();
    }
}

