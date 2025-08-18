const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe_mod = b.createModule(.{
        .target = target,
        .optimize = optimize,
    });

    const exe = b.addExecutable(.{
        .name = "xeltu",
        .root_module = exe_mod,
    });
    exe.addCSourceFiles(.{
        .files = &[_][]const u8{
            "src/debug.c",
            "src/evaluater.c",
            "src/main.c",
            "src/memory.c",
            "src/parser.c",
            "src/scanner.c",
            "src/table.c",
        },
        .flags = &[_][]const u8{
            "-Werror",
            "-Wall",
            "-Wno-unused-variable",
            "-Wno-unused-parameter",
            "-Wno-unused-function",
            "-g",
            "-glldb",
            "-std=c99"
        },
    });
    exe.linkLibC();

    b.installArtifact(exe);

    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }
    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);
}
