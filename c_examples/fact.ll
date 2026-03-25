; ModuleID = './c_examples/fact.cpp'
source_filename = "./c_examples/fact.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [35 x i8] c"Usage: 1 argument - factorial len\0A\00", align 1
@.str.1 = private unnamed_addr constant [17 x i8] c"Fact(%lu) = %lu\0A\00", align 1
@dump_dynamic_logger_info_path = private unnamed_addr constant [22 x i8] c"info/dynamic_info.bin\00", align 1
@llvm.global_dtors = appending global [1 x { i32, ptr, ptr }] [{ i32, ptr, ptr } { i32 0, ptr @__dtor_wrapper, ptr null }]

; Function Attrs: mustprogress noinline optnone sspstrong uwtable
define dso_local noundef i64 @_Z4factm(i64 noundef %0) #0 {
  call void @basic_block_start_logger(i64 2)
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  store i64 %0, ptr %2, align 8
  store i64 0, ptr %3, align 8
  %5 = load i64, ptr %2, align 8
  %6 = icmp ult i64 %5, 2
  br i1 %6, label %7, label %8

7:                                                ; preds = %1
  call void @basic_block_start_logger(i64 14)
  store i64 1, ptr %3, align 8
  br label %15

8:                                                ; preds = %1
  call void @basic_block_start_logger(i64 18)
  %9 = load i64, ptr %2, align 8
  %10 = sub i64 %9, 1
  call void @call_logger(i64 21, i64 1)
  %11 = call noundef i64 @_Z4factm(i64 noundef %10)
  call void @res_int_logger(i64 %11, i64 21)
  store i64 %11, ptr %4, align 8
  %12 = load i64, ptr %2, align 8
  %13 = load i64, ptr %4, align 8
  %14 = mul i64 %12, %13
  store i64 %14, ptr %3, align 8
  br label %15

15:                                               ; preds = %8, %7
  call void @basic_block_start_logger(i64 28)
  %16 = load i64, ptr %3, align 8
  ret i64 %16
}

; Function Attrs: mustprogress noinline norecurse optnone sspstrong uwtable
define dso_local noundef i32 @main(i32 noundef %0, ptr noundef %1) #1 {
  call void @basic_block_start_logger(i64 32)
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca ptr, align 8
  %6 = alloca i64, align 8
  store i32 0, ptr %3, align 4
  store i32 %0, ptr %4, align 4
  store ptr %1, ptr %5, align 8
  %7 = load i32, ptr %4, align 4
  %8 = icmp ne i32 %7, 2
  br i1 %8, label %9, label %12

9:                                                ; preds = %2
  call void @basic_block_start_logger(i64 47)
  call void @call_logger(i64 48, i64 82)
  %10 = call i32 (ptr, ...) @printf(ptr noundef @.str)
  %11 = sext i32 %10 to i64
  call void @res_int_logger(i64 %11, i64 48)
  store i32 1, ptr %3, align 4
  br label %33

12:                                               ; preds = %2
  call void @basic_block_start_logger(i64 52)
  %13 = load ptr, ptr %5, align 8
  %14 = getelementptr inbounds ptr, ptr %13, i64 1
  %15 = load ptr, ptr %14, align 8
  call void @call_logger(i64 56, i64 83)
  %16 = call i32 @atoi(ptr noundef %15) #5
  %17 = sext i32 %16 to i64
  call void @res_int_logger(i64 %17, i64 56)
  %18 = sext i32 %16 to i64
  store i64 %18, ptr %6, align 8
  call void @call_logger(i64 59, i64 84)
  %19 = call ptr @__errno_location() #6
  %20 = ptrtoint ptr %19 to i64
  call void @res_int_logger(i64 %20, i64 59)
  %21 = load i32, ptr %19, align 4
  %22 = icmp eq i32 %21, 0
  br i1 %22, label %23, label %29

23:                                               ; preds = %12
  call void @basic_block_start_logger(i64 65)
  %24 = load i64, ptr %6, align 8
  %25 = load i64, ptr %6, align 8
  call void @call_logger(i64 68, i64 1)
  %26 = call noundef i64 @_Z4factm(i64 noundef %25)
  call void @res_int_logger(i64 %26, i64 68)
  call void @call_logger(i64 69, i64 82)
  %27 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i64 noundef %24, i64 noundef %26)
  %28 = sext i32 %27 to i64
  call void @res_int_logger(i64 %28, i64 69)
  br label %32

29:                                               ; preds = %12
  call void @basic_block_start_logger(i64 72)
  call void @call_logger(i64 73, i64 82)
  %30 = call i32 (ptr, ...) @printf(ptr noundef @.str)
  %31 = sext i32 %30 to i64
  call void @res_int_logger(i64 %31, i64 73)
  store i32 1, ptr %3, align 4
  br label %33

32:                                               ; preds = %23
  call void @basic_block_start_logger(i64 76)
  store i32 0, ptr %3, align 4
  br label %33

33:                                               ; preds = %32, %29, %9
  call void @basic_block_start_logger(i64 79)
  %34 = load i32, ptr %3, align 4
  ret i32 %34
}

declare i32 @printf(ptr noundef, ...) #2

; Function Attrs: nounwind willreturn memory(read)
declare i32 @atoi(ptr noundef) #3

; Function Attrs: nounwind willreturn memory(none)
declare ptr @__errno_location() #4

declare void @call_logger(i64, i64)

declare void @res_int_logger(i64, i64)

declare void @basic_block_start_logger(i64)

define internal void @__dtor_wrapper() {
  call void @dump_dynamic_logger_info(ptr @dump_dynamic_logger_info_path)
  ret void
}

declare void @dump_dynamic_logger_info(ptr)

attributes #0 = { mustprogress noinline optnone sspstrong uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { mustprogress noinline norecurse optnone sspstrong uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind willreturn memory(read) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nounwind willreturn memory(none) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { nounwind willreturn memory(read) }
attributes #6 = { nounwind willreturn memory(none) }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 21.1.6"}
