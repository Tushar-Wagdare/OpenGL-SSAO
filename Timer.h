#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <unordered_map>
#include <time.h>


class Timer {
private:
    // Private constructor to prevent direct instantiation
    Timer() noexcept :
        m_StartTime(std::chrono::high_resolution_clock::now()),
        m_LastTime(m_StartTime),
        m_DeltaTime(0.0),
        m_TotalTime(0.0),
        m_FrameCount(0),
        m_FramesPerSecond(0.0),
        m_SecondsPerFrame(0.0),
        m_TimeScale(1.0),
        m_InitStartTime(m_StartTime)
    {}

    // Delete copy constructor and assignment
    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;

    // Singleton instance
    inline static Timer* instance = nullptr;

public:
    static clock_t start;

    static void startMyTimer(void)
    {
        start = clock();
    }

    static double getAppRunTime(void)
    {
        return(((double)(clock() - start)) / CLOCKS_PER_SEC);
    }

    // Singleton access
    static Timer& Get() {
        if (!instance) {
            instance = new Timer();
        }
        return *instance;
    }

    // Cleanup
    static void Shutdown() {
        delete instance;
        instance = nullptr;
    }

    // Reset all timer states
    static void Reset() noexcept {
        auto& timer = Get();
        timer.m_StartTime = std::chrono::high_resolution_clock::now();
        timer.m_LastTime = timer.m_StartTime;
        timer.m_DeltaTime = 0.0;
        timer.m_TotalTime = 0.0;
        timer.m_FrameCount = 0;
        timer.m_FramesPerSecond = 0.0;
        timer.m_SecondsPerFrame = 0.0;
        timer.m_InitializationTimes.clear();
    }

    // Initialization timing methods
    static void StartInit(const std::string& phase) noexcept {
        auto& timer = Get();
        timer.m_InitStartTime = std::chrono::high_resolution_clock::now();
        timer.m_CurrentInitPhase = phase;
    }

    static void EndInit() noexcept {
        auto& timer = Get();
        auto currentTime = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration<double>(currentTime - timer.m_InitStartTime).count();
        timer.m_InitializationTimes[timer.m_CurrentInitPhase] = duration;
    }

    // Get initialization time for a specific phase
    [[nodiscard]] static double GetInitTime(const std::string& phase) noexcept {
        auto& timer = Get();
        auto it = timer.m_InitializationTimes.find(phase);
        return (it != timer.m_InitializationTimes.end()) ? it->second : 0.0;
    }

    // Get total initialization time
    [[nodiscard]] static double GetTotalInitTime() noexcept {
        auto& timer = Get();
        double total = 0.0;
        for (const auto& [phase, time] : timer.m_InitializationTimes) {
            total += time;
        }
        return total;
    }

    // Update timer state - call once per frame
    static void Tick() noexcept {
        auto& timer = Get();
        auto currentTime = std::chrono::high_resolution_clock::now();
        timer.m_DeltaTime = std::chrono::duration<double>(currentTime - timer.m_LastTime).count() * timer.m_TimeScale;
        timer.m_LastTime = currentTime;
        timer.m_TotalTime = std::chrono::duration<double>(currentTime - timer.m_StartTime).count();
        ++timer.m_FrameCount;

        if (timer.m_TotalTime - timer.m_LastFPSUpdate >= 1.0) {
            timer.m_FramesPerSecond = static_cast<double>(timer.m_FrameCount) / (timer.m_TotalTime - timer.m_LastFPSUpdate);
            timer.m_SecondsPerFrame = 1.0 / timer.m_FramesPerSecond;
            timer.m_LastFPSUpdate = timer.m_TotalTime;
            timer.m_FrameCount = 0;
        }
    }

    // Getters
    [[nodiscard]] static double GetDeltaTime() noexcept { return Get().m_DeltaTime; }
    [[nodiscard]] static double GetTotalTime() noexcept { return Get().m_TotalTime; }
    [[nodiscard]] static double GetFramesPerSecond() noexcept { return Get().m_FramesPerSecond; }
    [[nodiscard]] static double GetSecondsPerFrame() noexcept { return Get().m_SecondsPerFrame; }
    [[nodiscard]] static uint64_t GetFrameCount() noexcept { return Get().m_FrameCount; }
    [[nodiscard]] static double GetTimeScale() noexcept { return Get().m_TimeScale; }

    // Setters
    static void SetTimeScale(double scale) noexcept { Get().m_TimeScale = scale; }

private:
    // Runtime performance members
    std::chrono::high_resolution_clock::time_point m_StartTime;
    std::chrono::high_resolution_clock::time_point m_LastTime;
    double m_DeltaTime;
    double m_TotalTime;
    uint64_t m_FrameCount;
    double m_FramesPerSecond;
    double m_SecondsPerFrame;
    double m_TimeScale;
    double m_LastFPSUpdate{ 0.0 };

    // Initialization timing members
    std::chrono::high_resolution_clock::time_point m_InitStartTime;
    std::string m_CurrentInitPhase;
    std::unordered_map<std::string, double> m_InitializationTimes;
};



// Convenience macros for timer access
#define TIMER_INIT(phase) Timer::StartInit(phase)
#define TIMER_END() Timer::EndInit()
#define TIMER_GET(phase) Timer::GetInitTime(phase)
#define TIMER_TOTAL_INIT() Timer::GetTotalInitTime()
#define TIMER_TICK() Timer::Tick()
#define TIMER_DELTA() Timer::GetDeltaTime()
#define TIMER_FPS() Timer::GetFramesPerSecond()
#define TIMER_SCALE(scale) Timer::SetTimeScale(scale)
#define TIMER_RESET() Timer::Reset()

#endif









// Example usage:
/*
int main() {
    // Initialization timing
    TIME_INIT("WindowInit");
    // ... window initialization code ...
    TIME_END();

    // Main game loop
    while (running) {
        TIME_TICK();
        float deltaTime = TIME_DELTA();

        // Slow motion effect
        if (slowMotion) {
            TIME_SCALE(0.5);
        }

        // Get performance metrics
        float fps = TIME_FPS();
        float initTime = TIME_GET("WindowInit");
    }

    Timer::Shutdown();
    return 0;
}
*/