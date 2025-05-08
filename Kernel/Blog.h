#pragma once
#include "Util.h"
#include "PondOS.h"


#define COMMENTS_DISK_OFFSET DISK_PROTECTED_AREA_END

#define BLOG_MEMORY_OFFSET 0x14000000000
B32 blogDataDirty;
U64 lastSerializeTime;

#define DISK_DIRTY_SERIALIZE_FREQUENCY_MILLIS 5000

// Because we have no ASLR, I can serialize pointers directly. Isn't that neat?

struct BlogComment {
	StrA username;
	StrA content;
	BlogComment* nextComment;
};
struct BlogEntry {
	StrA path;
	StrA content;
	BlogEntry* nextEntry;
	BlogComment* firstComment;
};
struct BlogPostsHeader {
	U64 dataSize;
	BlogEntry* firstEntry;
};

BlogEntry* get_blog_entry(StrA path) {
	for (BlogEntry* entry = ((BlogPostsHeader*)BLOG_MEMORY_OFFSET)->firstEntry; entry; entry = entry->nextEntry) {
		if (entry->path == path) {
			return entry;
		}
	}
	return nullptr;
}

template<typename T>
T* blog_alloc(U32 count = 1) {
	BlogPostsHeader* header = (BlogPostsHeader*)BLOG_MEMORY_OFFSET;
	U64 oldDataSize = header->dataSize;
	U64 result = ALIGN_HIGH(oldDataSize, alignof(T));
	U64 newDataSize = result + sizeof(T) * count;
	if (ALIGN_HIGH(oldDataSize, 0x1000) != ALIGN_HIGH(newDataSize, 0x1000)) {
		SyscallVirtualMapArgs mapArgs;
		mapArgs.address = BLOG_MEMORY_OFFSET + ALIGN_HIGH(oldDataSize, 0x1000);
		mapArgs.length = ALIGN_HIGH(newDataSize, 0x1000) - ALIGN_HIGH(oldDataSize, 0x1000);
		mapArgs.pageFlags = PAGE_PRESENT | PAGE_WRITE | PAGE_EXECUTE_DISABLE;
		if (g_syscallProc(SYSCALL_VIRTUAL_MAP, U64(&mapArgs)) != SYSCALL_VIRTUAL_MAP_SUCCESS) {
			print("Blog alloc failed\n");
			return nullptr;
		}
	}
	header->dataSize = newDataSize;
	blogDataDirty = true;
	return (T*)(BLOG_MEMORY_OFFSET + result);
}

StrA blog_make_stra(StrA str) {
	char* strData = blog_alloc<char>(str.length);
	memcpy(strData, str.str, str.length);
	return StrA{ strData, str.length };
}
void blog_add_comment(BlogEntry* entry, StrA username, StrA content) {
	BlogComment* comment = blog_alloc<BlogComment>();
	comment->username = blog_make_stra(username);
	comment->content = blog_make_stra(content);

	comment->nextComment = entry->firstComment;
	entry->firstComment = comment;
	blogDataDirty = true;
}
void blog_add_entry(StrA path, StrA content) {
	BlogEntry* entry = blog_alloc<BlogEntry>();
	entry->path = blog_make_stra(path);
	entry->content = blog_make_stra(content);

	BlogPostsHeader* header = (BlogPostsHeader*)BLOG_MEMORY_OFFSET;
	entry->nextEntry = header->firstEntry;
	header->firstEntry = entry;
	blogDataDirty = true;
}

void init_blog() {
	BlogPostsHeader* header = (BlogPostsHeader*)BLOG_MEMORY_OFFSET;
	header->dataSize = sizeof(BlogPostsHeader);
	header->firstEntry = nullptr;
	blog_add_entry("/BuildingThisBlog.html"a, R"(
<h1>Building This Blog, Part 1</h1>
<h2>This Blog</h2>
<p>Every programmer needs a blog, right? I've wanted one for years now, a place to post my thoughts on programming, development logs for my game, random ideas, etc. Really a very simple idea, how hard could it be?</p>
<h2>Web Technology?</h2>
<p>Building a blog, however, requires web development, something I started out completely unfamiliar with. So, how does one go about learning web development? There's the site builders, of course, places like blogspot.com, Wix, Weebly, and so on, but these give you relatively little control, and I felt these would not fit my needs.</p>
<p>Going a step lower, we have WordPress, which is used everywhere, probably in most of the programming blogs I've visited. This still didn't fit my needs; I wanted more control and I didn't like the idea of using a website builder. I wanted my website to be 100% mine.</p>
<p>Lower than that, we have frameworks, technology like React, Vue, Angular, and so on. I very much did not want to use any frameworks because I thought they would abstract too much, leaving me with knowledge of a specific framework and not much else. Frameworks also tend to be extremely bloated, slowing websites down, potentially even causing them to take multiple seconds to load. I wanted my blog to load quickly.</p>
<p>This leaves bare HTML, CSS, and Javascript. I could learn this. There's just one small issue: once I write it, how do I attach this data to a domain name such that anyone with a browser can download it and view it?</p>
<h2>How Does One Get HTML to a Browser?</h2>
<p>Again, there are multiple ways to handle this. First candidate: GitHub Pages. Very simple to set up, easy to manage the source code, handles all the hosting for you, you just need to point your domain name at it. Big issue: no dynamic content. I wanted comments on my blog site for sure, so GitHub Pages was out.</p>
<p>Second candidate: Linux server in the cloud with off-the-shelf hosting software like Nginx or Apache. Seems like a decent choice, but I felt I wasn't going to truly understand how my HTML was getting to the browser if I just used some pre-made program to handle the site.</p>
<p>Third candidate: Custom program that uses a library to receive HTTP requests so that I can respond to them manually. But, I thought to myself, I don't know how the HTTP library works, so I still wouldn't understand what the server is doing!</p>
<p>Fourth candidate: Custom program that uses unix sockets to receive TCP messages and parse HTTP requests from them. Now we're getting somewhere, but I still won't understand how the browser is connecting to the computer over TCP.</p>
<p>Final candidate: At some point, I realized that the only way I was going to be happy was if I implemented everything I possibly could myself to make sure I understood the whole web server front to back; I needed to build an operating system.</p>
<h2>The S in HTTPS?</h2>
<p>There's still one major connection component to deal with, and that's encryption. Everything is expected to be encrypted these days, and blogs are no exception. Even if there's no real reason to cryptographically protect the contents of this blog, which doesn't even have so much as an account system, browsers expect it, which meant I needed to learn the necessary cryptographic protocols and the math behind them.</p>
<h2>Databases?</h2>
<p>I mentioned wanting comments earlier, which means I needed some sort of way to store comments per post. I couldn't exactly use a normal database on a custom OS (and I wouldn't want to anyway, the whole point is that I know how everything works in this server), so I needed to make something up to store comments. I felt this would be a great opportunity to explore file systems, so I decided to implement a file system, loosely based on NTFS and ext4.</p>
<h2>Conclusion</h2>
<p>So there we go, the work was cut out. I needed to, from the OS up, implement a full HTTPS web server hosting a blog with comments. I'll dive deeper into my experience implementing each part in future posts.</p>
)"a);
	blog_add_entry("/TestPost.html"a, R"(
<h1>Test Post Header</h1>
<h2>Some test subheader</h2>
<p>A paragraph</p>
<h2>Some other test subheader</h2>
<p>Another paragraph</p>
)"a);
	blog_add_entry("/Posts.html"a, R"(
<h1>List of Blog Posts</h1>
<p><a href="/TestPost.html">Test Post</a></p>
<p><a href="/TestPost.html">Building This Blog, Part 1</a></p>
)"a);
	blog_add_entry("/About.html"a, R"(
<h1>Welcome to the DrillEngine Blog</h1>
<p>This is my pet web dev project, build from the ground up. Everything from the bootloader to the HTTP server is made by me with no libraries.</p>
)"a);
	blog_add_entry("/MainPage.html"a, R"(
<h1>One more programming blog</h1>
<p>Welcome! This is where I'll post programming stuff. Check out the Posts page for more.</p>
)"a);
	blogDataDirty = true;
}

void disk_serialize_blog() {
	print("Serializing blog...");
	BlogPostsHeader* header = (BlogPostsHeader*)BLOG_MEMORY_OFFSET;
	SyscallDiskCommandBlockingArgs diskArgs;
	diskArgs.type = DISK_CMD_TYPE_WRITE;
	diskArgs.sectorCount = ALIGN_HIGH(header->dataSize, 512) / 512;
	diskArgs.data = (void*)BLOG_MEMORY_OFFSET;
	diskArgs.diskSectorOffset = COMMENTS_DISK_OFFSET / 512;
	if (g_syscallProc(SYSCALL_DISK_COMMAND_BLOCKING, U64(&diskArgs)) != DISK_ERROR_SUCCESS) {
		print("failed\n");
	} else {
		print("serialized\n");
	}
	blogDataDirty = false;
}
B32 disk_deserialize_blog() {
	SyscallVirtualMapArgs mapArgs;
	mapArgs.address = BLOG_MEMORY_OFFSET;
	mapArgs.length = 0x1000;
	mapArgs.pageFlags = PAGE_PRESENT | PAGE_WRITE | PAGE_EXECUTE_DISABLE;
	if (g_syscallProc(SYSCALL_VIRTUAL_MAP, U64(&mapArgs)) != SYSCALL_VIRTUAL_MAP_SUCCESS) {
		print("Blog memory map header failed\n");
		return false;
	}
	SyscallDiskCommandBlockingArgs diskArgs;
	diskArgs.type = DISK_CMD_TYPE_READ;
	diskArgs.sectorCount = 1;
	diskArgs.data = (void*)BLOG_MEMORY_OFFSET;
	diskArgs.diskSectorOffset = COMMENTS_DISK_OFFSET / 512;
	DiskError diskErr = DiskError(g_syscallProc(SYSCALL_DISK_COMMAND_BLOCKING, U64(&diskArgs)));
	if (diskErr != DISK_ERROR_SUCCESS) {
		print("Blog disk header read failed\n");
		print_num(diskErr);
		return false;
	}
	BlogPostsHeader* header = (BlogPostsHeader*)BLOG_MEMORY_OFFSET;
	U64 dataSize = header->dataSize;
	if (dataSize == 0) {
		print("Blog not present, initializing\n");
		init_blog();
		return true;
	}
	mapArgs.address = BLOG_MEMORY_OFFSET;
	mapArgs.length = dataSize;
	mapArgs.pageFlags = PAGE_PRESENT | PAGE_WRITE | PAGE_EXECUTE_DISABLE;
	if (g_syscallProc(SYSCALL_VIRTUAL_MAP, U64(&mapArgs)) != SYSCALL_VIRTUAL_MAP_SUCCESS) {
		print("Blog memory map failed\n");
		g_syscallProc(SYSCALL_SHUTDOWN, SYSCALL_SHUTDOWN_CODE_DEEP_SLEEP);
	}
	diskArgs.type = DISK_CMD_TYPE_READ;
	diskArgs.sectorCount = ALIGN_HIGH(dataSize, 512) / 512;
	diskArgs.data = (void*)BLOG_MEMORY_OFFSET;
	diskArgs.diskSectorOffset = COMMENTS_DISK_OFFSET / 512;
	diskErr = DiskError(g_syscallProc(SYSCALL_DISK_COMMAND_BLOCKING, U64(&diskArgs)));
	if (diskErr != DISK_ERROR_SUCCESS) {
		print("Blog disk read failed\n");
		print_num(diskErr);
		return false;
	}
	print("Blog deserialized\n");
	return true;
}

void blog_write_dirty_data() {
	U64 currentTime = current_time_millis();
	if (blogDataDirty && currentTime > lastSerializeTime + DISK_DIRTY_SERIALIZE_FREQUENCY_MILLIS) {
		disk_serialize_blog();
		lastSerializeTime = current_time_millis();
	}
}