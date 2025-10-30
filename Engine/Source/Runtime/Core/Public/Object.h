#pragma once
#include "BEngineCoreAPI.h"
#include <string>
#include <atomic>
#include <type_traits>
#include <utility>

class BENGINE_CORE_API BObject {
public:
    explicit BObject(BObject* inOuter = nullptr)
        : Id(NextId.fetch_add(1, std::memory_order_relaxed)), Outer(inOuter) {}

    virtual ~BObject() = default;

    // Identity
    uint64_t GetId() const { return Id; }
    const std::string& GetName() const { return Name; }
    void Rename(std::string n) { Name = std::move(n); }

    // Class name (simple RTTI helper)
    virtual const char* GetClassName() const { return "BObject"; }

    // Ownership (UE-style "Outer")
    BObject* GetOuter() const { return Outer; }
    template<typename T> T* GetOuterAs() const { return dynamic_cast<T*>(Outer); }

    // Helpers
    template<typename T> bool IsA() const { return dynamic_cast<const T*>(this) != nullptr; }
    template<typename T> T* Cast() { return dynamic_cast<T*>(this); }
    template<typename T> const T* Cast() const { return dynamic_cast<const T*>(this); }

    // Factory
    template<typename T, typename...Args>
    static T* NewObject(BObject* outer, Args&&...args) {
        static_assert(std::is_base_of_v<BObject, T>, "T must derive from BObject");
        T* obj = new T(std::forward<Args>(args)...);
        obj->Outer = outer;
        if (obj->Name.empty()) obj->Name = std::string(T::StaticClassName()) + "_" + std::to_string(obj->Id);
        return obj;
    }

protected:
    void SetDefaultName(const std::string& base) {
        if (Name.empty()) Name = base + "_" + std::to_string(Id);
    }

private:
    uint64_t    Id;
    std::string Name;
    BObject*    Outer = nullptr;
    static std::atomic<uint64_t> NextId;
};

// Simple macro to give classes a static + virtual class name
#define BCLASS(ClassType) \
public: \
static const char* StaticClassName() { return #ClassType; } \
const char* GetClassName() const override { return #ClassType; }
