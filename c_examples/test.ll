; ModuleID = './c_examples/test.cpp'
source_filename = "./c_examples/test.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [15 x i8] c"Hello, world!\0A\00", align 1
@dump_dynamic_logger_info_path = private unnamed_addr constant [22 x i8] c"info/dynamic_info.bin\00", align 1
@llvm.global_dtors = appending global [1 x { i32, ptr, ptr }] [{ i32, ptr, ptr } { i32 0, ptr @__dtor_wrapper, ptr null }]

; Function Attrs: mustprogress noinline norecurse optnone sspstrong uwtable
define dso_local noundef i32 @main() #0 {
  call void @basic_block_start_logger(i64 93984985269648)
  call void @call_logger(i64 93984985208928, i64 93984985208584)
  %1 = call i32 (ptr, ...) @printf(ptr noundef @.str)
  %2 = sext i32 %1 to i64
  call void @call_logger(i64 93984985640224, i64 93984985629736)
  call void @res_int_logger(i64 %2, i64 93984985208928)
  ret i32 0
}

declare i32 @printf(ptr noundef, ...) #1

declare void @call_logger(i64, i64)

declare void @res_int_logger(i64, i64)

declare void @basic_block_start_logger(i64)

define internal void @__dtor_wrapper() {
  call void @dump_dynamic_logger_info(ptr @dump_dynamic_logger_info_path)
  ret void
}

declare void @dump_dynamic_logger_info(ptr)

attributes #0 = { mustprogress noinline norecurse optnone sspstrong uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 21.1.6"}
