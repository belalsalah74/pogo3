group = "app.pokemongo"

patches {
    about {
        name = "Pokémon GO Patches"
        description = "Morphe patches for Pokémon GO: Google web sign-in for re-signed builds, " +
            "plus an experimental native throw hook."
        source = "git@github.com:belalsalah74/pogo1.git"
        author = "belalsalah74"
        contact = "https://github.com/belalsalah74/pogo1/issues"
        website = "https://github.com/belalsalah74/pogo1"
        license = "GPLv3"
    }
}

// Separate configuration so gson is available at runtime for the
// generatePatchesList task but never bundled into the APK.
val patchListGeneratorClasspath = configurations.create("patchListGeneratorClasspath")

dependencies {
    compileOnly(libs.gson)
    patchListGeneratorClasspath(libs.gson)
}

tasks {
    register<JavaExec>("generatePatchesList") {
        description = "Build patch with patch list"

        dependsOn(build)

        classpath = sourceSets["main"].runtimeClasspath + patchListGeneratorClasspath
        mainClass.set("util.PatchListGeneratorKt")
    }

    // Used by gradle-semantic-release-plugin.
    publish {
        dependsOn("generatePatchesList")
    }
}
