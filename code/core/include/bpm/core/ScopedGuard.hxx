
#ifndef BPM_CORE_SCOPED_GUARD_HXX
#define BPM_CORE_SCOPED_GUARD_HXX

namespace bpm
{
   namespace core
   {
      template <typename F>
      class ScopeGuard
      {
         public:
         
            explicit ScopeGuard(F&& f)
               : cleanup_(std::forward<F>(f))
               , active_(true)
            {
            }
    
            ~ScopeGuard()
            {
               if (active_) cleanup_();
            }

            ScopeGuard(const ScopeGuard&) = delete;
         
            ScopeGuard& operator=(const ScopeGuard&) = delete;
    
            ScopeGuard(ScopeGuard&&) = delete;
            
            ScopeGuard& operator=(ScopeGuard&&) = delete;

            void dismiss()
            {
               active_ = false;
            }

         private:
    
            F cleanup_;
            bool active_;

      };

      template <typename F>
      ScopeGuard<F> makeScopedGuard(F&& f)
      {
         return ScopeGuard<F>(std::forward<F>(f));
      }
   }
}

#endif
