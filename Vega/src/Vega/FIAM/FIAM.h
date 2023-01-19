#pragma once

#include <iostream>
#include <mutex>
#include <functional>
#include <vector>
#include <unordered_map>

#include "Vega/Core/Base.h"
#include "Vega/Debug/ConsoleLog.h"

namespace LM
{

    struct MaterialFIAM
    {
        double sigmaB;
        double sigma02;
        double delta;
        double HB;
        double E1;
        double u1;
        double kM;
    };

    struct DataFIAM
    {
        double Rzi = 20.0;
        double Rzb = 0.8;
        double lambda = -5.0;
        double phi = 60.0;
        double phi1 = 5.0;
        double gamma = 20.0;
        double r = 1.2;
        double rho  = 0.01;
        double hz = 0.2;
        double v = 400.0;
        double t = 1;
        double s = 0.2;
    };

    struct ResultFIAM
    {
        double h1 = 0.0;
        double h2 = 0.0;
        double h3 = 0.0;
        double h4 = 0.0;
        double Rz = 0.0;
    };

    class FIAM
    {
    protected:
        enum class Status
        {
            kStart,
            kError,
            kOk
        };
    public:
        FIAM();
        void OnImGuiDraw();

        static void Init() { s_Instance = CreateRef<FIAM>(); LOGW("\n\n\n\n\n\n\n\nREF CREATED! \n\n\n\n\n\n\n\n"); }
        static Ref<FIAM> Get() { return s_Instance; }
    protected:
        void SetMaterialNameId(std::string _MatName);
        void DrawInputs();
        void DrawResult();
        void DrawH1();
        void DrawH2();
        void DrawH3();
        void DrawRz();
        void DrawSchemes();
        
        void GetMaterialNames();
        
        void Calculate();
        void CalculateCommon(ResultFIAM& _Result, double s);
        void Calculate1(ResultFIAM& _Result, double s, double bs);
        void Calculate2(ResultFIAM& _Result, double s, double bs);
        void Calculate3(ResultFIAM& _Result, double s, double bs);
        void Calculate4(ResultFIAM& _Result, double s, double bs);
        void Calculate5(ResultFIAM& _Result, double s, double bs);

        std::function<void(ResultFIAM&, double, double)> GetCalcFunc(int& _Scheme, double s);
    protected:
        static inline const std::string Mat_AMr6 = "AMr6";
        static inline const std::string Mat_D16  = "D16";
        static inline const std::string Mat_20   = "20";
        static inline const std::string Mat_45   = "45";
    protected:
        DataFIAM m_Data;

        std::unordered_map<std::string, MaterialFIAM> m_Materials;
        std::vector<const char*> m_MaterialNames;
        int m_SelectedMaterialId = 0;

        int m_Scheme = 1;
        ResultFIAM m_Result;
        std::vector<ResultFIAM> m_GraphicResults;
        std::vector<std::vector<double>> m_Graphics;
        std::vector<double> m_GraphicSchemes;
        Status m_Status = Status::kStart;

        std::mutex m_Mtx;
        static inline Ref<FIAM> s_Instance = nullptr;
    };

}
