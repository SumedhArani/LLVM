; ModuleID = 'input.c'
source_filename = "input.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.11.0"

%struct.ST = type { i32, double, %struct.RT }
%struct.RT = type { i8, [10 x [20 x i32]], i8 }

; Function Attrs: nounwind ssp uwtable
define i8* @foo(%struct.ST* %s) #0 {
entry:
  %s.addr = alloca %struct.ST*, align 8
  store %struct.ST* %s, %struct.ST** %s.addr, align 8
  %0 = load %struct.ST*, %struct.ST** %s.addr, align 8
  %arrayidx = getelementptr inbounds %struct.ST, %struct.ST* %0, i64 1
  %Z = getelementptr inbounds %struct.ST, %struct.ST* %arrayidx, i32 0, i32 2
  %A = getelementptr inbounds %struct.RT, %struct.RT* %Z, i32 0, i32 0
  ret i8* %A
}

attributes #0 = { nounwind ssp uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="core2" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"PIC Level", i32 2}
!1 = !{!"clang version 3.9.0 (http://llvm.org/git/clang.git 44a5e636a46ebaf333fc9e9e18863f67bbb1a1ed) (http://llvm.org/git/llvm.git ff7edbe38c28426980827e63ba2e528fd44da1e1)"}
