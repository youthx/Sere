; ModuleID = '<string>'
source_filename = "<string>"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@strtmp_2 = constant [14 x i8] c"Hello, world!\00"

; Function Attrs: mustprogress nofree norecurse nosync nounwind readnone willreturn
define void @__global__() local_unnamed_addr #0 {
  ret void
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind readnone willreturn
define i32 @main() local_unnamed_addr #0 {
  ret i32 12
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind readnone willreturn
define i32 @foo(i32 %0, i32 %1) local_unnamed_addr #0 {
  %3 = add i32 %1, %0
  ret i32 %3
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind readnone willreturn }
