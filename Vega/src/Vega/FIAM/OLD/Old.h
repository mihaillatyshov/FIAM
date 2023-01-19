#include "FIAM.h"


#include "imgui.h"
#include "glm.hpp"

#include "Vega/Debug/ConsoleLog.h"
#include "Vega/ImGui/Plots/implot.h"

namespace LM
{

    std::string operator"" S(const char8_t* str, std::size_t) {
        return reinterpret_cast<const char*>(str);
    }
    char const* operator"" C(const char8_t* str, std::size_t) {
        return reinterpret_cast<const char*>(str);
    }

    FIAM::FIAM()
    {
        m_Materials = std::vector<MaterialFIAM>({
            {"AMr6", 315.0, 130.0, 15.0, 650.0, 71.0, 0.33, 0.95},
            {"D16", 400.0, 290.0, 9.0, 1050.0, 72.0, 0.33, 0.95},
            {"20", 490.0, 294.0, 25.0, 1630.0, 212.0f, 0.27, 0.925},
            //{"45"},
            });
        GetMaterialNames();
    }

    void FIAM::GetMaterialNames()
    {
        for (const auto& material : m_Materials)
        {
            m_MaterialNames.push_back(material.name.data());
        }
    }

    void FIAM::OnImGuiDraw()
    {
        DrawInputs();
        DrawResult();
    }

    void FIAM::DrawInputs()
    {
        if (ImGui::Begin("FIAM Inputs"))
        {
            ImGui::Text("Material");
            ImGui::Combo("combo", &m_SelectedMaterialId, &m_MaterialNames[0], m_MaterialNames.size());
            ImGui::Text("sigmaB: %lf", m_Materials[m_SelectedMaterialId].sigmaB);
            ImGui::Text("sigma02: %lf", m_Materials[m_SelectedMaterialId].sigma02);
            ImGui::Text("delta: %lf", m_Materials[m_SelectedMaterialId].delta);
            ImGui::Text("HB: %lf", m_Materials[m_SelectedMaterialId].HB);
            ImGui::Text("E1: %lf", m_Materials[m_SelectedMaterialId].E1);
            ImGui::Text("u1: %lf", m_Materials[m_SelectedMaterialId].u1);
            ImGui::Text("kM: %lf", m_Materials[m_SelectedMaterialId].kM);
            ImGui::Separator();

            ImGui::Text("Data");
            ImGui::InputDouble("Rzi", &m_Data.Rzi);
            ImGui::InputDouble("Rzb", &m_Data.Rzb);
            ImGui::InputDouble("lambda", &m_Data.lambda);
            ImGui::InputDouble("phi", &m_Data.phi);
            ImGui::InputDouble("phi1", &m_Data.phi1);
            ImGui::InputDouble("gamma", &m_Data.gamma);
            ImGui::InputDouble("r", &m_Data.r);
            ImGui::InputDouble("rho", &m_Data.rho);
            ImGui::InputDouble("hz", &m_Data.hz);
            ImGui::InputDouble("v", &m_Data.v);
            ImGui::InputDouble("t", &m_Data.t);
            ImGui::InputDouble("s", &m_Data.s);
            ImGui::Separator();
            if (ImGui::Button("Calculate"))
            {
                Calculate();
            }
            ImGui::Separator();
            if (ImGui::Button("Var 9"))
            {
                m_Data = { 40.0, 0.7, 5.0, 45.0, 10.0, 12.0, 0.6, 0.027, 0.5, 240.0, 2.0, 0.3 };
            }
        }
        ImGui::End();
    }

    void FIAM::DrawResult()
    {
        if (ImGui::Begin("FIAM Result"))
        {
            ImGui::Text("Result");
            ImGui::SameLine();
            if (ImGui::Button("Calculate"))
            {
                Calculate();
            }
            switch (m_Status)
            {
            case Status::kStart:
                ImGui::Text("Click Calculate!");
                break;
            case Status::kError:
                ImGui::Text("You got some ERROR!!!");
                break;
            case Status::kOk:
                ImGui::Text("Schema: %d", m_Scheme);
                ImGui::Text("h1: %lf", m_Result.h1);
                ImGui::Text("h2: %lf", m_Result.h2);
                ImGui::Text("h3: %lf", m_Result.h3);
                ImGui::Text("h4: %lf", m_Result.h4);
                ImGui::Text("Rz: %lf", m_Result.Rz);
                DrawH1();
                DrawH2();
                DrawH3();
                DrawRz();
                DrawSchemes();
                break;
            default:
                break;
            }
        }
        ImGui::End();
    }

    void FIAM::DrawH1()
    {
        if (ImGui::Begin("h1"))
        {
            if (ImPlot::BeginPlot("h1", ImVec2(-1, 600)))
            {
                ImPlot::PlotLine("h1", m_Graphics[0].data(), m_Graphics[1].data(), m_Graphics[0].size());
                ImPlot::EndPlot();
            }
        }
        ImGui::End();
    }

    void FIAM::DrawH2()
    {
        if (ImGui::Begin("h2"))
        {
            if (ImPlot::BeginPlot("h2", ImVec2(-1, 600)))
            {
                ImPlot::PlotLine("h2", m_Graphics[0].data(), m_Graphics[2].data(), m_Graphics[0].size());
                ImPlot::EndPlot();
            }
        }
        ImGui::End();
    }

    void FIAM::DrawH3()
    {
        if (ImGui::Begin("h3"))
        {
            if (ImPlot::BeginPlot("h3", ImVec2(-1, 600)))
            {
                ImPlot::PlotLine("h3", m_Graphics[0].data(), m_Graphics[3].data(), m_Graphics[0].size());
                ImPlot::EndPlot();
            }
        }
        ImGui::End();
    }

    void FIAM::DrawRz()
    {
        if (ImGui::Begin("Rz"))
        {
            if (ImPlot::BeginPlot("Rz", ImVec2(-1, 600)))
            {
                ImPlot::PlotLine("Rz", m_Graphics[0].data(), m_Graphics[4].data(), m_Graphics[0].size());
                ImPlot::EndPlot();
            }
        }
        ImGui::End();
    }

    void FIAM::DrawSchemes()
    {
        if (ImGui::Begin("Schemes"))
        {
            if (ImPlot::BeginPlot("Schemes", ImVec2(-1, 600)))
            {
                ImPlot::PlotLine("Schemes", m_Graphics[0].data(), m_GraphicSchemes.data(), m_GraphicSchemes.size());
                ImPlot::EndPlot();
            }
        }
        ImGui::End();
    }

    std::function<void(ResultFIAM&, double, double)> FIAM::GetCalcFunc(int& _Scheme, double s)
    {
        using namespace std::placeholders;

        if (m_Data.r >= 0 && m_Data.r <= (s / 2.0))
        {
            _Scheme = 0;
            return std::bind(&FIAM::Calculate1, this, _1, _2, _3);
        }
        else if ((glm::radians(m_Data.phi) >= glm::asin(s / (2.0 * m_Data.r))) && (glm::radians(m_Data.phi1) >= glm::asin(s / (2.0 * m_Data.r))))
        {
            _Scheme = 1;
            return std::bind(&FIAM::Calculate2, this, _1, _2, _3);
        }
        else if ((glm::radians(m_Data.phi) > glm::asin(s / (2.0 * m_Data.r))) && (glm::radians(m_Data.phi1) <= glm::asin(s / (2.0 * m_Data.r))))
        {
            _Scheme = 2;
            return std::bind(&FIAM::Calculate3, this, _1, _2, _3);
        }
        else if ((glm::radians(m_Data.phi) <= glm::asin(s / (2.0 * m_Data.r))) && (glm::radians(m_Data.phi1) > glm::asin(s / (2.0 * m_Data.r))))
        {
            _Scheme = 3;
            return std::bind(&FIAM::Calculate4, this, _1, _2, _3);
        }
        else
        {
            _Scheme = 4;
            return std::bind(&FIAM::Calculate5, this, _1, _2, _3);
        }
    }

    void FIAM::Calculate()
    {
        using namespace std::placeholders;

        try
        {
            std::function<void(ResultFIAM&, double, double)> CalcFunc = GetCalcFunc(m_Scheme, m_Data.s);

            double tauS = 0.75 * m_Materials[m_SelectedMaterialId].sigmaB;
            double sigma02 = m_Materials[m_SelectedMaterialId].sigma02;
            double bs = 1000.0 * 0.5 * m_Data.rho * (1.0 - tauS / glm::sqrt(tauS * tauS + sigma02 * sigma02));
            LOGE("TauS: ", tauS);
            LOGE("BS: ", bs);

            CalcFunc(m_Result, m_Data.s, bs);
            CalculateCommon(m_Result, m_Data.s);

            m_GraphicResults.clear();
            m_Graphics.clear();
            m_GraphicSchemes.clear();
            m_Graphics.emplace_back();
            m_Graphics.emplace_back();
            m_Graphics.emplace_back();
            m_Graphics.emplace_back();
            m_Graphics.emplace_back();
            for (double s = 0.08; s < 5.001; s += 0.005)
            {
                int scheme;
                CalcFunc = GetCalcFunc(scheme, s);
                m_GraphicSchemes.emplace_back(scheme);
                CalcFunc(m_GraphicResults.emplace_back(), s, bs);
                CalculateCommon(m_GraphicResults.back(), s);
                m_Graphics[0].emplace_back(s);
                m_Graphics[1].emplace_back(m_GraphicResults.back().h1);
                m_Graphics[2].emplace_back(m_GraphicResults.back().h2);
                m_Graphics[3].emplace_back(m_GraphicResults.back().h3);
                m_Graphics[4].emplace_back(m_GraphicResults.back().Rz);
            }

            m_Status = Status::kOk;
        }
        catch (...)
        {
            m_Status = Status::kError;
        }
    }


    void FIAM::CalculateCommon(ResultFIAM& _Result, double s)
    {
        _Result.h4 = m_Data.Rzb;
        _Result.h2 = 0;

        double Kv = 3.5 * glm::pow(m_Data.v, -0.25);
        double Kh = 0.96;
        _Result.Rz = (1000.0 * _Result.h1 + _Result.h2 + _Result.h3 + _Result.h4) * Kv * (1 + Kh * m_Data.hz);
    }

    void FIAM::Calculate1(ResultFIAM& _Result, double s, double bs)
    {
        _Result.h1 = s * ((glm::sin(glm::radians(m_Data.phi)) * glm::sin(glm::radians(m_Data.phi1)))
            / (glm::sin(glm::radians(m_Data.phi)) + glm::sin(glm::radians(m_Data.phi1))));

        _Result.h3 = bs / ((1.0 / glm::tan(glm::radians(m_Data.phi))) + (1.0 / glm::tan(glm::radians(m_Data.phi1))));
    }

    void FIAM::Calculate2(ResultFIAM& _Result, double s, double bs)
    {
        _Result.h1 = s * s / (8.0 * m_Data.r);

        _Result.h3 = bs * (2.0 * s + bs) / (32.0 * m_Data.r);
    }

    void FIAM::Calculate3(ResultFIAM& _Result, double s, double bs)
    {
        double phi0 = glm::acos((m_Data.r - s * glm::sin(glm::radians(m_Data.phi1)) / (m_Data.r))) - glm::radians(m_Data.phi1);

        _Result.h1 = m_Data.r * (1.0 - glm::cos(glm::radians(phi0)));

        _Result.h3 = (bs) / ((1.0 / glm::tan(glm::radians(m_Data.phi1))) + ((2.0 * m_Data.r) / s));
    }

    void FIAM::Calculate4(ResultFIAM& _Result, double s, double bs)
    {
        double phi0 = glm::acos((m_Data.r - s * glm::sin(glm::radians(m_Data.phi)) / (m_Data.r))) - glm::radians(m_Data.phi);

        _Result.h1 = m_Data.r * (1.0 - glm::cos(glm::radians(phi0)));

        _Result.h3 = bs / ((1.0 / glm::tan(glm::radians(m_Data.phi))) + ((2.0 * m_Data.r) / s));
    }

    void FIAM::Calculate5(ResultFIAM& _Result, double s, double bs)
    {
        _Result.h1 = m_Data.r * (1.0 - glm::cos(glm::radians(m_Data.phi)))
            + ((glm::tan(glm::radians(m_Data.phi)) * glm::tan(glm::radians(m_Data.phi1)) * (s - m_Data.r * (glm::sin(glm::radians(m_Data.phi))
                + glm::sin(glm::radians(m_Data.phi1))))
                - (m_Data.r * glm::tan(glm::radians(m_Data.phi)) * (glm::cos(glm::radians(m_Data.phi))
                    - glm::cos(glm::radians(m_Data.phi)))))
                / (glm::tan(glm::radians(m_Data.phi)) + glm::tan(glm::radians(m_Data.phi1))));

        _Result.h3 = bs / ((1.0 / glm::tan(glm::radians(m_Data.phi))) + (1.0 / glm::tan(glm::radians(m_Data.phi1))));
    }

}
